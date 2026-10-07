# Projekt Big Data — ryzyko kredytowe (Spark + Kafka + MLlib)

Kompletny projekt akademicki spełniający wymagania przedmiotu **Big Data**:
przetwarzanie na dużą skalę w **Apache Spark**, strumień przez **Apache Kafka**,
klasyfikacja w **Spark MLlib**, notebook pod **Google Colab**, ścieżka na **Azure Databricks**.

**Temat:** detekcja ryzyka defaultu klientów kart kredytowych (UCI Default of Credit Card Clients).

## Szybki start

```bash
cd big-data-projekt
pip install -r requirements.txt
bash scripts/run_pipeline.sh
```

Pipeline wykona: ETL → symulację strumienia → scorowanie → trening MLlib → wykresy.

### Opcjonalnie: prawdziwa Kafka

```bash
docker compose up -d
python scripts/kafka_producer.py --limit 2000
python scripts/spark_streaming.py --live --kafka --seconds 60
```

### Google Colab

Otwórz `notebooks/BigData_Analiza_Colab.ipynb` i uruchom komórki (dane pobiorą się automatycznie).

## Sprawozdanie

Pełny opis celu, tez, doboru danych, ETL, ML i wniosków: **[SPRAWOZDANIE.md](SPRAWOZDANIE.md)**.

## Tezy

| Teza | Treść |
|------|--------|
| T1 | `PAY_*` silniej przewidują default niż sam limit |
| T2 | Wysoki `util_ratio` ↑ ryzyko |
| T3 | MLlib AUC > 0.70 |
| T4 | Kafka + Spark = wczesne ostrzeganie |

## Wymagania zaliczeniowe — mapowanie

| Wymaganie | Realizacja |
|-----------|------------|
| Apache Spark | `spark_etl.py`, `spark_ml.py`, `spark_streaming.py` |
| Streaming i/lub MLlib | **oba**: Structured Streaming + RandomForest |
| Apache Kafka | `docker-compose.yml` + `kafka_producer.py` |
| Google Colab | `notebooks/BigData_Analiza_Colab.ipynb` |
| Wizualizacje | `visualize.py` → `reports/figures/` |
| Sprawozdanie + źródła | `SPRAWOZDANIE.md` |
