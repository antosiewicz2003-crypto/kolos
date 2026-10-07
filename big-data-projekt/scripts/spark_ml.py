#!/usr/bin/env python3
"""
Uczenie maszynowe MLlib na klastrze Apache Spark:
  - pipeline: VectorAssembler → StandardScaler → RandomForestClassifier
  - ewaluacja: AUC, Accuracy, Precision, Recall, F1
  - zapis modelu i metryk

Uruchomienie:
  python scripts/spark_ml.py
"""

from __future__ import annotations

import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from pyspark.ml import Pipeline
from pyspark.ml.classification import LogisticRegression, RandomForestClassifier
from pyspark.ml.evaluation import BinaryClassificationEvaluator, MulticlassClassificationEvaluator
from pyspark.ml.feature import StandardScaler, VectorAssembler
from pyspark.sql import functions as F

from config.settings import FEATURE_COLS, FEATURES_PARQUET, FIGURES, LABEL_COL, MODELS
from scripts.spark_session import get_spark


def load_features(spark):
    df = spark.read.parquet(str(FEATURES_PARQUET))
    # Upewnij się, że wszystkie cechy istnieją
    missing = [c for c in FEATURE_COLS if c not in df.columns]
    if missing:
        raise RuntimeError(f"Brak cech po ETL: {missing}. Uruchom najpierw spark_etl.py")
    return df.select(*FEATURE_COLS, LABEL_COL)


def build_pipeline(algorithm: str = "rf") -> Pipeline:
    assembler = VectorAssembler(inputCols=FEATURE_COLS, outputCol="features_raw", handleInvalid="skip")
    scaler = StandardScaler(inputCol="features_raw", outputCol="features", withStd=True, withMean=False)

    if algorithm == "lr":
        clf = LogisticRegression(
            featuresCol="features",
            labelCol=LABEL_COL,
            maxIter=50,
            regParam=0.01,
            elasticNetParam=0.0,
        )
    else:
        clf = RandomForestClassifier(
            featuresCol="features",
            labelCol=LABEL_COL,
            numTrees=100,
            maxDepth=8,
            seed=42,
            impurity="gini",
        )

    return Pipeline(stages=[assembler, scaler, clf])


def evaluate(predictions):
    auc_eval = BinaryClassificationEvaluator(
        labelCol=LABEL_COL, rawPredictionCol="rawPrediction", metricName="areaUnderROC"
    )
    acc_eval = MulticlassClassificationEvaluator(
        labelCol=LABEL_COL, predictionCol="prediction", metricName="accuracy"
    )
    f1_eval = MulticlassClassificationEvaluator(
        labelCol=LABEL_COL, predictionCol="prediction", metricName="f1"
    )
    prec_eval = MulticlassClassificationEvaluator(
        labelCol=LABEL_COL, predictionCol="prediction", metricName="weightedPrecision"
    )
    rec_eval = MulticlassClassificationEvaluator(
        labelCol=LABEL_COL, predictionCol="prediction", metricName="weightedRecall"
    )

    metrics = {
        "auc": float(auc_eval.evaluate(predictions)),
        "accuracy": float(acc_eval.evaluate(predictions)),
        "f1": float(f1_eval.evaluate(predictions)),
        "precision": float(prec_eval.evaluate(predictions)),
        "recall": float(rec_eval.evaluate(predictions)),
    }

    # Macierz pomyłek
    cm = (
        predictions.groupBy(LABEL_COL, "prediction")
        .count()
        .orderBy(LABEL_COL, "prediction")
        .collect()
    )
    metrics["confusion"] = [
        {"label": int(r[LABEL_COL]), "prediction": int(r["prediction"]), "count": int(r["count"])}
        for r in cm
    ]
    return metrics


def feature_importance(model, algorithm: str):
    """Ważność cech z Random Forest (jeśli dostępna)."""
    try:
        rf_model = model.stages[-1]
        if hasattr(rf_model, "featureImportances"):
            imp = rf_model.featureImportances.toArray().tolist()
            ranked = sorted(
                [{"feature": f, "importance": float(v)} for f, v in zip(FEATURE_COLS, imp)],
                key=lambda x: x["importance"],
                reverse=True,
            )
            return ranked
    except Exception:
        pass
    return []


def main(algorithm: str = "rf"):
    spark = get_spark("CreditRiskMLlib")
    spark.sparkContext.setLogLevel("WARN")
    MODELS.mkdir(parents=True, exist_ok=True)
    FIGURES.mkdir(parents=True, exist_ok=True)

    try:
        df = load_features(spark)
        train, test = df.randomSplit([0.8, 0.2], seed=42)

        print(f"Train: {train.count()} | Test: {test.count()}")
        print(f"Algorytm: {algorithm}")

        pipeline = build_pipeline(algorithm)
        model = pipeline.fit(train)
        predictions = model.transform(test)

        metrics = evaluate(predictions)
        importance = feature_importance(model, algorithm)
        metrics["top_features"] = importance[:10]
        metrics["algorithm"] = algorithm
        metrics["n_train"] = train.count()
        metrics["n_test"] = test.count()

        model_path = MODELS / f"credit_risk_{algorithm}"
        model.write().overwrite().save(str(model_path))

        metrics_path = MODELS / f"metrics_{algorithm}.json"
        metrics_path.write_text(json.dumps(metrics, indent=2), encoding="utf-8")

        print("=" * 60)
        print("Wyniki MLlib")
        for k in ("auc", "accuracy", "f1", "precision", "recall"):
            print(f"  {k:12s}: {metrics[k]:.4f}")
        print(f"  Model zapisany: {model_path}")
        print(f"  Metryki:        {metrics_path}")
        if importance:
            print("  Top 5 cech:")
            for row in importance[:5]:
                print(f"    {row['feature']:20s} {row['importance']:.4f}")
        print("=" * 60)

        # Teza T3
        if metrics["auc"] > 0.70:
            print("TEZA T3 POTWIERDZONA: AUC > 0.70")
        else:
            print("TEZA T3 NIEPOTWIERDZONA: AUC <= 0.70")

        return metrics
    finally:
        spark.stop()


if __name__ == "__main__":
    algo = sys.argv[1] if len(sys.argv) > 1 else "rf"
    main(algo)
