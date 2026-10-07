"""Konfiguracja projektu Big Data — detekcja ryzyka kredytowego / default."""

from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]

DATA_RAW = PROJECT_ROOT / "data" / "raw"
DATA_PROCESSED = PROJECT_ROOT / "data" / "processed"
DATA_STREAM = PROJECT_ROOT / "data" / "stream_sim"
FIGURES = PROJECT_ROOT / "reports" / "figures"
MODELS = PROJECT_ROOT / "data" / "models"

RAW_CSV = DATA_RAW / "UCI_Credit_Card.csv"
CLEAN_PARQUET = DATA_PROCESSED / "credit_clean.parquet"
FEATURES_PARQUET = DATA_PROCESSED / "credit_features.parquet"
STREAM_JSONL = DATA_STREAM / "transactions.jsonl"

KAFKA_BOOTSTRAP = "localhost:9092"
KAFKA_TOPIC = "credit-transactions"

LABEL_COL = "default_payment_next_month"
ID_COL = "ID"

FEATURE_COLS = [
    "LIMIT_BAL",
    "SEX",
    "EDUCATION",
    "MARRIAGE",
    "AGE",
    "PAY_0",
    "PAY_2",
    "PAY_3",
    "PAY_4",
    "PAY_5",
    "PAY_6",
    "BILL_AMT1",
    "BILL_AMT2",
    "BILL_AMT3",
    "BILL_AMT4",
    "BILL_AMT5",
    "BILL_AMT6",
    "PAY_AMT1",
    "PAY_AMT2",
    "PAY_AMT3",
    "PAY_AMT4",
    "PAY_AMT5",
    "PAY_AMT6",
    "util_ratio",
    "avg_bill",
    "avg_pay",
    "pay_bill_ratio",
    "delay_count",
]

# Tezy projektu (do sprawozdania / notebooka)
THESES = [
    "T1: Historia opóźnień płatności (PAY_*) jest silniejszym predyktorem defaultu niż same limity kredytowe.",
    "T2: Wysoki wskaźnik wykorzystania limitu (BILL/LIMIT) koreluje dodatnio z ryzykiem defaultu.",
    "T3: Model klasyfikacyjny MLlib na klastrze Spark osiąga AUC > 0.70 na zbiorze testowym.",
    "T4: Strumieniowe scorowanie transakcji (Kafka → Spark Streaming) umożliwia wczesne ostrzeganie w czasie zbliżonym do rzeczywistego.",
]
