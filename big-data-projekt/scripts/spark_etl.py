#!/usr/bin/env python3
"""
ETL Apache Spark: ekstrakcja, porządkowanie, feature engineering, zapis Parquet.
Uruchomienie:
  python scripts/spark_etl.py
  # lub: spark-submit scripts/spark_etl.py
"""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from pyspark.sql import functions as F
from pyspark.sql.types import DoubleType, IntegerType

from config.settings import (
    CLEAN_PARQUET,
    DATA_PROCESSED,
    FEATURES_PARQUET,
    LABEL_COL,
    RAW_CSV,
)
from scripts.spark_session import get_spark


def extract(spark):
    """Wczytanie surowego CSV UCI Credit Card."""
    df = (
        spark.read.option("header", True)
        .option("inferSchema", True)
        .csv(str(RAW_CSV))
    )
    # Ujednolicenie nazwy etykiety
    if "default.payment.next.month" in df.columns:
        df = df.withColumnRenamed("default.payment.next.month", LABEL_COL)
    return df


def transform(df):
    """Porządkowanie, walidacja i modelowanie cech."""
    # Typy liczbowe
    for c in df.columns:
        if c != "ID":
            df = df.withColumn(c, F.col(c).cast(DoubleType()))

    df = df.withColumn("ID", F.col("ID").cast(IntegerType()))

    # EDUCATION: 0,5,6 → 4 (other); MARRIAGE: 0 → 3 (other)
    df = df.withColumn(
        "EDUCATION",
        F.when(F.col("EDUCATION").isin(0, 5, 6), 4).otherwise(F.col("EDUCATION")),
    ).withColumn(
        "MARRIAGE",
        F.when(F.col("MARRIAGE") == 0, 3).otherwise(F.col("MARRIAGE")),
    )

    # Usunięcie rekordów z brakującym limitem / etykietą
    df = df.filter(F.col("LIMIT_BAL").isNotNull() & F.col(LABEL_COL).isNotNull())
    df = df.filter(F.col("LIMIT_BAL") > 0)

    bill_cols = [f"BILL_AMT{i}" for i in range(1, 7)]
    pay_cols = [f"PAY_AMT{i}" for i in range(1, 7)]
    delay_cols = ["PAY_0", "PAY_2", "PAY_3", "PAY_4", "PAY_5", "PAY_6"]

    df = df.withColumn("avg_bill", sum(F.col(c) for c in bill_cols) / 6.0)
    df = df.withColumn("avg_pay", sum(F.col(c) for c in pay_cols) / 6.0)
    df = df.withColumn(
        "util_ratio",
        F.when(F.col("LIMIT_BAL") > 0, F.col("BILL_AMT1") / F.col("LIMIT_BAL")).otherwise(0.0),
    )
    df = df.withColumn(
        "pay_bill_ratio",
        F.when(F.col("avg_bill") != 0, F.col("avg_pay") / F.col("avg_bill")).otherwise(0.0),
    )
    # Liczba miesięcy z opóźnieniem (PAY_* > 0)
    delay_expr = sum(F.when(F.col(c) > 0, 1).otherwise(0) for c in delay_cols)
    df = df.withColumn("delay_count", delay_expr.cast(DoubleType()))

    # Znacznik czasu symulowanego zdarzenia (do streaming)
    df = df.withColumn(
        "event_ts",
        F.from_unixtime(F.lit(1_704_067_200) + F.col("ID") * 3).cast("timestamp"),
    )

    return df


def load(df):
    """Zapis do Parquet (warstwa curated)."""
    DATA_PROCESSED.mkdir(parents=True, exist_ok=True)
    (
        df.write.mode("overwrite")
        .parquet(str(CLEAN_PARQUET))
    )
    feature_df = df.drop("event_ts")
    (
        feature_df.write.mode("overwrite")
        .parquet(str(FEATURES_PARQUET))
    )
    return df


def summarize(df):
    total = df.count()
    defaults = df.filter(F.col(LABEL_COL) == 1).count()
    print("=" * 60)
    print("ETL — podsumowanie")
    print(f"  Rekordów:          {total}")
    print(f"  Default (1):       {defaults} ({100 * defaults / total:.2f}%)")
    print(f"  Non-default (0):   {total - defaults} ({100 * (total - defaults) / total:.2f}%)")
    print(f"  Parquet clean:     {CLEAN_PARQUET}")
    print(f"  Parquet features:  {FEATURES_PARQUET}")
    print("=" * 60)
    df.select(
        "LIMIT_BAL", "AGE", "util_ratio", "delay_count", "avg_bill", LABEL_COL
    ).describe().show()


def main():
    spark = get_spark("CreditRiskETL")
    spark.sparkContext.setLogLevel("WARN")
    try:
        raw = extract(spark)
        print(f"Surowy schemat ({len(raw.columns)} kolumn):")
        raw.printSchema()
        clean = transform(raw)
        load(clean)
        summarize(clean)
    finally:
        spark.stop()


if __name__ == "__main__":
    main()
