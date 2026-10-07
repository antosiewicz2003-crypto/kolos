#!/usr/bin/env bash
# Pełny pipeline lokalny (bez obowiązkowej Kafki)
set -euo pipefail
cd "$(dirname "$0")/.."

echo "==> [1/4] ETL Spark"
python3 scripts/spark_etl.py

echo "==> [2/4] Producent strumienia (JSONL / Kafka jeśli dostępna)"
python3 scripts/kafka_producer.py --limit 2000 --file-only

echo "==> [3/4] Streaming demo + scorowanie"
python3 scripts/spark_streaming.py

echo "==> [4/4] MLlib RandomForest + wizualizacje"
python3 scripts/spark_ml.py rf
python3 scripts/visualize.py

echo "Pipeline zakończony."
