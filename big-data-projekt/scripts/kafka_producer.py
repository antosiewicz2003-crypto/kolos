#!/usr/bin/env python3
"""
Producent Apache Kafka: symuluje strumień zdarzeń transakcyjnych
na podstawie danych UCI Credit Card.

Tryby:
  1) Kafka (domyślnie) — wymaga docker compose up -d
  2) Fallback plikowy — gdy broker niedostępny, zapis JSONL do data/stream_sim/

Uruchomienie:
  python scripts/kafka_producer.py [--limit 500] [--delay 0.05] [--file-only]
"""

from __future__ import annotations

import argparse
import json
import sys
import time
from datetime import datetime, timezone
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import pandas as pd

from config.settings import (
    DATA_STREAM,
    KAFKA_BOOTSTRAP,
    KAFKA_TOPIC,
    LABEL_COL,
    RAW_CSV,
    STREAM_JSONL,
)


def row_to_event(row: dict, seq: int) -> dict:
    """Mapowanie rekordu historycznego na zdarzenie strumieniowe."""
    label_key = "default.payment.next.month"
    default = int(row.get(label_key, row.get(LABEL_COL, 0)))
    return {
        "event_id": f"txn-{seq:06d}",
        "client_id": int(row["ID"]),
        "ts": datetime.now(timezone.utc).isoformat(),
        "limit_bal": float(row["LIMIT_BAL"]),
        "sex": int(row["SEX"]),
        "education": int(row["EDUCATION"]),
        "marriage": int(row["MARRIAGE"]),
        "age": int(row["AGE"]),
        "pay_status": [int(row[f"PAY_{i}"]) if f"PAY_{i}" in row else int(row["PAY_0"]) for i in (0, 2, 3, 4, 5, 6)],
        "bill_amt": [float(row[f"BILL_AMT{i}"]) for i in range(1, 7)],
        "pay_amt": [float(row[f"PAY_AMT{i}"]) for i in range(1, 7)],
        "label_default": default,  # tylko do ewaluacji offline; w produkcji niedostępne
    }


def try_kafka_producer():
    try:
        from kafka import KafkaProducer
        from kafka.errors import NoBrokersAvailable

        producer = KafkaProducer(
            bootstrap_servers=KAFKA_BOOTSTRAP,
            value_serializer=lambda v: json.dumps(v).encode("utf-8"),
            key_serializer=lambda k: str(k).encode("utf-8"),
            request_timeout_ms=5000,
            api_version_auto_timeout_ms=5000,
        )
        # szybki test połączenia
        producer.bootstrap_connected()
        return producer
    except Exception as exc:
        print(f"[WARN] Kafka niedostępna ({exc}). Przełączam na tryb plikowy.")
        return None


def produce(limit: int, delay: float, file_only: bool):
    df = pd.read_csv(RAW_CSV)
    if limit > 0:
        df = df.head(limit)

    DATA_STREAM.mkdir(parents=True, exist_ok=True)
    producer = None if file_only else try_kafka_producer()

    file_handle = open(STREAM_JSONL, "w", encoding="utf-8")
    sent = 0
    try:
        for seq, (_, row) in enumerate(df.iterrows(), start=1):
            event = row_to_event(row.to_dict(), seq)
            line = json.dumps(event, ensure_ascii=False)
            file_handle.write(line + "\n")

            if producer is not None:
                producer.send(KAFKA_TOPIC, key=event["client_id"], value=event)

            sent += 1
            if sent % 100 == 0:
                print(f"  Wysłano {sent} zdarzeń...")
            if delay > 0:
                time.sleep(delay)

        if producer is not None:
            producer.flush()
            print(f"[OK] Kafka topic '{KAFKA_TOPIC}': {sent} wiadomości")
        print(f"[OK] Plik strumienia: {STREAM_JSONL} ({sent} linii)")
    finally:
        file_handle.close()
        if producer is not None:
            producer.close()


def main():
    parser = argparse.ArgumentParser(description="Producent strumienia transakcji kredytowych")
    parser.add_argument("--limit", type=int, default=1000, help="Liczba zdarzeń (0 = wszystkie)")
    parser.add_argument("--delay", type=float, default=0.0, help="Opóźnienie między zdarzeniami [s]")
    parser.add_argument("--file-only", action="store_true", help="Tylko zapis JSONL, bez Kafki")
    args = parser.parse_args()
    produce(args.limit, args.delay, args.file_only)


if __name__ == "__main__":
    main()
