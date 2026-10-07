# Sprawozdanie z projektu Big Data

**Temat:** Detekcja ryzyka niewypłacalności klientów kart kredytowych z użyciem Apache Spark, Apache Kafka oraz Spark MLlib  

**Przedmiot:** Big Data  
**Narzędzia:** Apache Spark (ETL + Structured Streaming), Spark MLlib, Apache Kafka, Google Colab / lokalny Jupyter, (opcjonalnie) Azure Databricks  

---

## 1. Cel projektu i założenia

### 1.1. Cel

Celem projektu jest zbudowanie kompletu procesów Big Data — od pozyskania i porządkowania danych, przez strumieniowe scorowanie zdarzeń, po uczenie modelu klasyfikacyjnego — ilustrującego zastosowanie poszczególnych elementów ekosystemu Apache Spark w praktycznym scenariuszu **oceny ryzyka kredytowego (default)**.

Wyniki mają wspierać proces decyzyjny instytucji finansowej: wczesne ostrzeganie o klientach wysokiego ryzyka, priorytetyzację działań windykacyjnych oraz kalibrację limitów kredytowych.

### 1.2. Tezy badawcze

| Id | Teza | Metoda weryfikacji |
|----|------|--------------------|
| **T1** | Historia opóźnień płatności (`PAY_*`) jest silniejszym predyktorem defaultu niż same limity kredytowe. | Eksploracja (odsetek defaultów vs `PAY_0`) + ważność cech Random Forest (MLlib) |
| **T2** | Wysoki wskaźnik wykorzystania limitu (`BILL_AMT1 / LIMIT_BAL`) koreluje dodatnio z ryzykiem defaultu. | Agregacje statystyczne w przedziałach `util_ratio` |
| **T3** | Model klasyfikacyjny MLlib na Spark osiąga **AUC > 0.70** na zbiorze testowym. | Pipeline RF + `BinaryClassificationEvaluator` |
| **T4** | Strumieniowe scorowanie (Kafka → Spark) umożliwia wczesne ostrzeganie w czasie zbliżonym do rzeczywistego. | Producent Kafka / JSONL + scorowanie regułowe + precision dla klasy HIGH |

### 1.3. Założenia metodologiczne i narzędziowe

- **Statystyka opisowa:** rozkłady, balans klas, korelacje, odsetki warunkowe.
- **Eksploracja (EDA):** wizualizacje seaborn/matplotlib; identyfikacja nieprawidłowych kodowań (`EDUCATION`, `MARRIAGE`).
- **ETL na Spark:** ekstrakcja CSV → transformacje SQL/DataFrame → zapis Parquet (warstwa curated).
- **Uczenie maszynowe:** klasyfikacja binarna (default / brak) — Random Forest i opcjonalnie regresja logistyczna w Spark MLlib; podział 80/20.
- **Streaming:** Apache Kafka jako broker zdarzeń transakcyjnych; Spark Structured Streaming (oraz wariant demo batch z JSONL o schemacie Kafki) do scorowania ryzyka.
- **Prezentacja:** notebook Google Colab + wykresy w `reports/figures/`; środowisko chmurowe Azure Databricks jako docelowe miejsce wdrożenia (Spark + Delta Lake).

---

## 2. Dobór danych

### 2.1. Źródło i uzasadnienie wyboru

Wybrano zbiór **Default of Credit Card Clients** z repozytorium UCI Machine Learning Repository (Yeh, Lien, 2009) — dane klientów kart kredytowych z Tajwanu (kwiecień–wrzesień 2005).

**Uzasadnienie:**
- publiczny, dobrze udokumentowany zbiór (dane wtórne z badań akademickich / instytucji finansowych),
- etykieta `default payment next month` umożliwia nadzorowaną klasyfikację (MLlib),
- cechy czasowe historii płatności (`PAY_0`…`PAY_6`, rachunki, spłaty) naturalnie mapują się na **strumień zdarzeń** (Kafka),
- skala 30 000 rekordów × 24 atrybuty jest wystarczająca do demonstracji Spark, a pipeline jest skalowalny na większe wolumeny.

Źródło: <https://archive.ics.uci.edu/dataset/350/default-of-credit-card-clients>  
Plik roboczy CSV: `data/raw/UCI_Credit_Card.csv` (mirror Hugging Face / UCI).

### 2.2. Struktura i formaty natywne

| Element | Opis |
|---------|------|
| Format źródłowy | Excel (`.xls`) w UCI; w projekcie użyto CSV (UTF-8, nagłówek) |
| Format roboczy | CSV → Parquet (Spark) |
| Format strumienia | JSON (wiadomości Kafka / JSONL) |
| Liczba wierszy | 30 000 klientów |
| Etykieta | `default.payment.next.month` ∈ {0, 1} |

**Główne atrybuty:**
- demografia: `SEX`, `EDUCATION`, `MARRIAGE`, `AGE`
- limit: `LIMIT_BAL`
- historia statusu płatności: `PAY_0` … `PAY_6` (−2…8; wartości dodatnie = opóźnienie)
- rachunki: `BILL_AMT1` … `BILL_AMT6`
- spłaty: `PAY_AMT1` … `PAY_AMT6`

### 2.3. Charakter ilościowy i jakościowy danych surowych

**Ilościowy:**
- ~22% klientów z defaultem (klasa mniejszościowa) — typowy problem niezbalansowany,
- limity od kilku tysięcy do ~1 mln jednostek NT$,
- wiek klientów głównie 20–60 lat.

**Jakościowy:**
- dane **wtórne** (zarejestrowane przez instytucję finansową, opublikowane do badań),
- nie są to dane pierwotne zbierane przez autorów projektu,
- występują niestandardowe kody (np. `EDUCATION` ∈ {0,5,6}, `MARRIAGE` = 0) wymagające porządkowania.

**Dynamika zmian:**
- w oryginale: **rejestracja w interwałach miesięcznych** (6 miesięcy historii),
- w projekcie: historyczne rekordy są **replay’owane jako strumień zdarzeń** (ciągła rejestracja / symulacja czasu rzeczywistego przez Kafkę),
- jednorazowy snapshot etykiety: default w następnym miesiącu.

### 2.4. Proces pozyskiwania strumienia przez Apache Kafka

1. Uruchomienie brokera: `docker compose up -d` (Zookeeper + Kafka na `localhost:9092`).
2. Producent `scripts/kafka_producer.py` czyta CSV i publikuje wiadomości na topic `credit-transactions` w formacie JSON:
   - `event_id`, `client_id`, `ts`, limity, statusy `pay_status[]`, `bill_amt[]`, `pay_amt[]`.
3. Równolegle (i jako fallback bez Dockera) zdarzenia są zapisywane do `data/stream_sim/transactions.jsonl`.
4. Konsument Spark (`scripts/spark_streaming.py`):
   - tryb `--live --kafka`: Structured Streaming z Kafki,
   - tryb domyślny: demo batch na JSONL o tym samym schemacie (reprodukowalne w Colab/CI).
5. Każde zdarzenie otrzymuje `risk_score` / `risk_level` (LOW / MEDIUM / HIGH) na podstawie reguł biznesowych zgodnych z tezami T1–T2.

---

## 3. Ekstrakcja i analiza danych

### 3.1. Porządkowanie, przekształcanie i modelowanie

**Porządkowanie:**
- ujednolicenie nazwy etykiety → `default_payment_next_month`,
- mapowanie nietypowych kodów edukacji i stanu cywilnego,
- filtr `LIMIT_BAL > 0`, rzutowanie typów.

**Feature engineering (modelowanie cech):**
- `util_ratio` = `BILL_AMT1 / LIMIT_BAL` — wykorzystanie limitu (teza T2),
- `avg_bill`, `avg_pay`, `pay_bill_ratio` — zachowania spłatowe,
- `delay_count` — liczba miesięcy z opóźnieniem (teza T1).

**Techniki odkrywania prawidłowości:**
- statystyki opisowe Spark (`describe`),
- odsetki warunkowe defaultu vs `PAY_0` / przedziały `util_ratio`,
- macierz korelacji,
- ważność cech lasu losowego (ukryte zależności nieliniowe trudne do zauważenia „ręcznie” przy 24+ cechach).

### 3.2. Skrypty ETL / Streaming / ML (Apache Spark)

| Skrypt | Rola |
|--------|------|
| `scripts/spark_etl.py` | Extract CSV → Transform features → Load Parquet |
| `scripts/kafka_producer.py` | Publikacja strumienia do Kafki / JSONL |
| `scripts/spark_streaming.py` | Scorowanie strumienia, agregacje, summary |
| `scripts/spark_ml.py` | Pipeline MLlib (RF / LR), metryki, zapis modelu |
| `scripts/visualize.py` | Wykresy do sprawozdania |
| `scripts/run_pipeline.sh` | Orchestracja lokalna end-to-end |
| `notebooks/BigData_Analiza_Colab.ipynb` | Notebook Colab z pełną ścieżką analizy |

Uruchomienie:

```bash
cd big-data-projekt
pip install -r requirements.txt
bash scripts/run_pipeline.sh
```

### 3.3. Proces uczenia maszynowego na klastrze Spark (MLlib)

**Zadanie:** klasyfikacja binarna — czy klient dopuści się defaultu w następnym miesiącu.

**Pipeline:**
1. `VectorAssembler` — wektor cech,
2. `StandardScaler` — skalowanie,
3. `RandomForestClassifier` (`numTrees=100`, `maxDepth=8`) — model zespołowy odporny na nieliniowości i różne skale cech.

**Ewaluacja (zbiór testowy ~20%):**
- AUC-ROC, Accuracy, Precision, Recall, F1,
- macierz pomyłek,
- ranking ważności cech.

**Wyniki pomiaru (lokalny run, RandomForest, seed=42):**

| Metryka | Wartość |
|---------|---------|
| AUC-ROC | **0.787** |
| Accuracy | 0.816 |
| F1 | 0.794 |
| Precision (weighted) | 0.797 |
| Recall (weighted) | 0.816 |
| Train / Test | 24 032 / 5 968 |

**Top 5 cech (ważność RF):** `PAY_0` (0.30), `delay_count` (0.20), `PAY_2`, `PAY_3`, `PAY_4` — **teza T1 potwierdzona**.  
**Teza T3 potwierdzona** (AUC > 0.70).

**Streaming (2000 zdarzeń replay):** HIGH=183, MEDIUM=460, LOW=1357; rule accuracy ≈ 0.77; precision HIGH ≈ 0.44 (reguły ostrzegawcze — czułe, do kalibracji progu).

Opcjonalnie: `python scripts/spark_ml.py lr` — regresja logistyczna jako model bazowy (baseline).

### 3.4. Azure Databricks (ścieżka wdrożeniowa)

Projekt lokalny / Colab jest przenaszalny na Azure Databricks:
- notebooki PySpark z tego repozytorium,
- dane curated jako **Delta Lake** zamiast zwykłego Parquet,
- Jobs do harmonogramu ETL + trenowania,
- Model Registry do wersjonowania modelu RF,
- Event Hubs / Kafka jako źródło streamu produkcyjnego.

---

## 4. Uwagi i wnioski końcowe (wsparcie decyzji)

1. **T1 potwierdzona:** najważniejsze cechy modelu to `PAY_0` i `delay_count`; odsetek defaultów rośnie monotonicznie wraz z opóźnieniem płatności (wykres `03_pay_status_default_rate.png`).
2. **T2 potwierdzona eksploracyjnie:** wyższe przedziały `util_ratio` wiążą się z wyższym odsetkiem defaultów (`04_util_ratio.png`).
3. **T3 potwierdzona:** AUC ≈ 0.79 — model nadaje się jako baza scorecardu kredytowego na Spark.
4. **T4 zademonstrowana:** Kafka/JSONL → scorowanie ryzyka w mikro-batchach; agregacje godzinowe gotowe pod dashboard KPI.
5. **Niezbalansowanie klas:** warto rozważyć wagowanie klas / oversampling w kolejnej iteracji (nie zmienia to architektury Big Data).
6. **Użyteczność biznesowa:**
   - **Credit risk:** decyzje o limicie i prowizji,
   - **Collections:** priorytet windykacji dla `risk_level=HIGH`,
   - **Monitoring portfela:** agregacje minutowe/godzinowe z streamu jako KPI dashboardu.

**Ograniczenia:** dane historyczne z 2005 (Tajwan); scorowanie strumieniowe w demo łączy reguły z replayem — w produkcji wymagany byłby model online / feature store i monitoring driftu.

---

## 5. Struktura repozytorium

```
big-data-projekt/
├── SPRAWOZDANIE.md          ← ten dokument
├── README.md
├── requirements.txt
├── docker-compose.yml       ← Kafka + Zookeeper
├── config/settings.py
├── data/raw/UCI_Credit_Card.csv
├── data/processed/          ← Parquet po ETL
├── data/stream_sim/         ← JSONL (fallback Kafki)
├── data/models/             ← model MLlib + metryki JSON
├── notebooks/BigData_Analiza_Colab.ipynb
├── reports/figures/         ← wykresy
└── scripts/
    ├── spark_etl.py
    ├── spark_ml.py
    ├── spark_streaming.py
    ├── kafka_producer.py
    ├── visualize.py
    └── run_pipeline.sh
```

---

## 6. Źródła

1. Yeh, I. C., & Lien, C. H. (2009). *The comparisons of data mining techniques for the predictive accuracy of probability of default of credit card clients*. Expert Systems with Applications, 36(2), 2473–2480.
2. Yeh, I. (2009). *Default of Credit Card Clients* [Dataset]. UCI Machine Learning Repository. https://doi.org/10.24432/C55S3H
3. Apache Spark Documentation — SQL, Structured Streaming, MLlib. https://spark.apache.org/docs/latest/
4. Apache Kafka Documentation. https://kafka.apache.org/documentation/
5. Databricks — Lakehouse & Delta Lake. https://docs.databricks.com/
6. Google Colaboratory. https://colab.research.google.com/
7. UCI Machine Learning Repository — datasets index. https://archive.ics.uci.edu/ml/datasets.html

---

*Dokument stanowi część składową oddania projektu Big Data (sprawozdanie + kod + wizualizacje).*
