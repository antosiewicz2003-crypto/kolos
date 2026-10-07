"""Wspólna fabryka sesji Spark."""

from pyspark.sql import SparkSession


def get_spark(app_name: str = "BigDataCreditRisk", master: str = "local[*]") -> SparkSession:
    return (
        SparkSession.builder.appName(app_name)
        .master(master)
        .config("spark.sql.shuffle.partitions", "8")
        .config("spark.driver.memory", "2g")
        .config("spark.sql.session.timeZone", "UTC")
        .getOrCreate()
    )
