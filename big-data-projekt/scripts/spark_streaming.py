#!/usr/bin/env python3
"""
Spark Structured Streaming: konsumpcja strumienia transakcji,
agregacje okienne i scorowanie ryzyka (reguły + opcjonalnie model MLlib).

Źródła wejścia (kolejność preferencji):
  1) Kafka (jeśli --kafka i broker dostępny)
  2) Plik JSONL (symulacja katalogu — rate source / textFile stream)

Uruchomienie:
  python scripts/spark_streaming.py --seconds 30
  python scripts/spark_streaming.py --kafka --seconds 60
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from pyspark.sql import functions as F
from pyspark.sql.types import (
    ArrayType,
    DoubleType,
    IntegerType,
    StringType,
    StructField,
    StructType,
)

from config.settings import (
    DATA_PROCESSED,
    DATA_STREAM,
    KAFKA_BOOTSTRAP,
    KAFKA_TOPIC,
    MODELS,
    STREAM_JSONL,
)
from scripts.spark_session import get_spark

EVENT_SCHEMA = StructType(
    [
        StructField("event_id", StringType()),
        StructField("client_id", IntegerType()),
        StructField("ts", StringType()),
        StructField("limit_bal", DoubleType()),
        StructField("sex", IntegerType()),
        StructField("education", IntegerType()),
        StructField("marriage", IntegerType()),
        StructField("age", IntegerType()),
        StructField("pay_status", ArrayType(IntegerType())),
        StructField("bill_amt", ArrayType(DoubleType())),
        StructField("pay_amt", ArrayType(DoubleType())),
        StructField("label_default", IntegerType()),
    ]
)


def parse_events(raw_df):
    """Parsuje JSON z kolumny value / value_str."""
    col = "value" if "value" in raw_df.columns else "value_str"
    parsed = raw_df.select(F.from_json(F.col(col).cast("string"), EVENT_SCHEMA).alias("e")).select("e.*")
    return (
        parsed.withColumn("event_time", F.to_timestamp("ts"))
        .withColumn("bill1", F.element_at("bill_amt", 1))
        .withColumn("pay0", F.element_at("pay_status", 1))
        .withColumn(
            "util_ratio",
            F.when(F.col("limit_bal") > 0, F.col("bill1") / F.col("limit_bal")).otherwise(0.0),
        )
        .withColumn(
            "delay_flag",
            F.when(F.col("pay0") > 0, 1).otherwise(0),
        )
        .withColumn(
            "risk_score",
            # prosty scoruję regułowy (do demo streamingu); MLlib używany w batch
            (
                F.when(F.col("util_ratio") > 0.8, 0.35).otherwise(0.0)
                + F.when(F.col("pay0") >= 2, 0.40).otherwise(F.when(F.col("pay0") == 1, 0.20).otherwise(0.0))
                + F.when(F.col("age") < 25, 0.10).otherwise(0.0)
                + F.when(F.col("limit_bal") < 50000, 0.15).otherwise(0.0)
            ),
        )
        .withColumn(
            "risk_level",
            F.when(F.col("risk_score") >= 0.55, "HIGH")
            .when(F.col("risk_score") >= 0.30, "MEDIUM")
            .otherwise("LOW"),
        )
    )


def read_kafka_stream(spark):
    return (
        spark.readStream.format("kafka")
        .option("kafka.bootstrap.servers", KAFKA_BOOTSTRAP)
        .option("subscribe", KAFKA_TOPIC)
        .option("startingOffsets", "earliest")
        .load()
        .selectExpr("CAST(value AS STRING) as value")
    )


def read_file_stream(spark):
    """Micro-batch z pliku JSONL (symulacja bez Kafki)."""
    # Upewnij się, że plik istnieje
    DATA_STREAM.mkdir(parents=True, exist_ok=True)
    if not STREAM_JSONL.exists():
        raise FileNotFoundError(
            f"Brak {STREAM_JSONL}. Uruchom najpierw: python scripts/kafka_producer.py --file-only"
        )
    return (
        spark.readStream.format("text")
        .option("maxFilesPerTrigger", 1)
        .load(str(DATA_STREAM))
        .withColumnRenamed("value", "value_str")
    )


def run_batch_demo(spark, seconds: int):
    """
    Demo bez nieskończonego streamu: odczyt JSONL jako batch + okna czasowe.
    Przydatne w CI / Colab bez długo działającego klastra.
    """
    if not STREAM_JSONL.exists():
        raise FileNotFoundError(f"Brak {STREAM_JSONL}")

    raw = spark.read.text(str(STREAM_JSONL)).withColumnRenamed("value", "value_str")
    events = parse_events(raw)

    print("=" * 60)
    print("Streaming (batch demo z JSONL / Kafka-compatible schema)")
    print(f"  Zdarzeń: {events.count()}")
    events.groupBy("risk_level").count().orderBy("risk_level").show()

    # Agregacja „okienna” po godzinie zdarzenia
    windowed = (
        events.withColumn("hour", F.date_trunc("hour", "event_time"))
        .groupBy("hour", "risk_level")
        .agg(
            F.count("*").alias("n_events"),
            F.avg("risk_score").alias("avg_risk"),
            F.avg("util_ratio").alias("avg_util"),
        )
        .orderBy("hour", "risk_level")
    )
    print("Agregacje godzinowe:")
    windowed.show(20, truncate=False)

    out = DATA_PROCESSED / "streaming_risk_summary"
    (
        events.select(
            "event_id",
            "client_id",
            "event_time",
            "limit_bal",
            "util_ratio",
            "pay0",
            "risk_score",
            "risk_level",
            "label_default",
        )
        .write.mode("overwrite")
        .parquet(str(out))
    )

    # Zgodność reguł z etykietą (przybliżona walidacja tezy T4)
    scored = events.withColumn(
        "pred_default", F.when(F.col("risk_level") == "HIGH", 1).otherwise(0)
    )
    total = scored.count()
    hits = scored.filter(F.col("pred_default") == F.col("label_default")).count()
    high = scored.filter(F.col("risk_level") == "HIGH").count()
    high_true = scored.filter((F.col("risk_level") == "HIGH") & (F.col("label_default") == 1)).count()

    summary = {
        "events": total,
        "rule_accuracy": round(hits / total, 4) if total else 0,
        "high_risk_events": high,
        "high_risk_true_defaults": high_true,
        "high_risk_precision": round(high_true / high, 4) if high else 0,
        "output": str(out),
    }
    summary_path = MODELS / "streaming_summary.json"
    MODELS.mkdir(parents=True, exist_ok=True)
    summary_path.write_text(json.dumps(summary, indent=2), encoding="utf-8")
    print("Podsumowanie scorowania strumieniowego:")
    print(json.dumps(summary, indent=2))
    print("=" * 60)
    return summary


def run_structured_streaming(spark, use_kafka: bool, seconds: int):
    raw = read_kafka_stream(spark) if use_kafka else read_file_stream(spark)
    events = parse_events(raw)

    agg = (
        events.withWatermark("event_time", "2 minutes")
        .groupBy(F.window("event_time", "1 minute"), "risk_level")
        .agg(F.count("*").alias("n"), F.avg("risk_score").alias("avg_risk"))
    )

    query = (
        agg.writeStream.outputMode("update")
        .format("console")
        .option("truncate", False)
        .trigger(processingTime="5 seconds")
        .start()
    )
    query.awaitTermination(seconds)
    query.stop()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--kafka", action="store_true", help="Czytaj z Apache Kafka")
    parser.add_argument("--live", action="store_true", help="Uruchom Structured Streaming (live)")
    parser.add_argument("--seconds", type=int, default=30, help="Czas działania live stream [s]")
    args = parser.parse_args()

    spark = get_spark("CreditRiskStreaming")
    spark.sparkContext.setLogLevel("WARN")
    try:
        if args.live:
            run_structured_streaming(spark, args.kafka, args.seconds)
        else:
            run_batch_demo(spark, args.seconds)
    finally:
        spark.stop()


if __name__ == "__main__":
    main()
