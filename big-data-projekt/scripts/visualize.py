#!/usr/bin/env python3
"""
Graficzna prezentacja wyników analizy (matplotlib/seaborn).
Generuje wykresy do reports/figures/ — do wklejenia w sprawozdanie / Colab.
"""

from __future__ import annotations

import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

from config.settings import FIGURES, LABEL_COL, MODELS, RAW_CSV

sns.set_theme(style="whitegrid", context="notebook")


def load_df() -> pd.DataFrame:
    df = pd.read_csv(RAW_CSV)
    df = df.rename(columns={"default.payment.next.month": LABEL_COL})
    return df


def fig_class_balance(df: pd.DataFrame):
    fig, ax = plt.subplots(figsize=(7, 4))
    counts = df[LABEL_COL].value_counts().sort_index()
    ax.bar(["Brak defaultu (0)", "Default (1)"], counts.values, color=["#2A6F97", "#C1121F"])
    ax.set_ylabel("Liczba klientów")
    ax.set_title("Balans klas — default płatności")
    for i, v in enumerate(counts.values):
        ax.text(i, v + 200, str(v), ha="center")
    fig.tight_layout()
    path = FIGURES / "01_class_balance.png"
    fig.savefig(path, dpi=140)
    plt.close(fig)
    return path


def fig_age_limit(df: pd.DataFrame):
    fig, axes = plt.subplots(1, 2, figsize=(11, 4))
    sns.histplot(data=df, x="AGE", hue=LABEL_COL, bins=25, ax=axes[0], palette=["#2A6F97", "#C1121F"])
    axes[0].set_title("Rozkład wieku vs default")
    sns.boxplot(
        data=df, x=LABEL_COL, y="LIMIT_BAL", hue=LABEL_COL,
        ax=axes[1], palette=["#2A6F97", "#C1121F"], legend=False,
    )
    axes[1].set_title("Limit kredytowy vs default")
    axes[1].set_xlabel("default")
    fig.tight_layout()
    path = FIGURES / "02_age_limit.png"
    fig.savefig(path, dpi=140)
    plt.close(fig)
    return path


def fig_pay_status(df: pd.DataFrame):
    fig, ax = plt.subplots(figsize=(8, 4))
    rates = df.groupby("PAY_0")[LABEL_COL].mean().sort_index()
    ax.plot(rates.index, rates.values, marker="o", color="#C1121F", linewidth=2)
    ax.set_xlabel("Status płatności PAY_0 (miesiąc bieżący)")
    ax.set_ylabel("Odsetek defaultów")
    ax.set_title("Teza T1: opóźnienia płatności a ryzyko defaultu")
    ax.set_ylim(0, 1)
    fig.tight_layout()
    path = FIGURES / "03_pay_status_default_rate.png"
    fig.savefig(path, dpi=140)
    plt.close(fig)
    return path


def fig_util_ratio(df: pd.DataFrame):
    util = (df["BILL_AMT1"] / df["LIMIT_BAL"]).clip(-1, 3)
    tmp = pd.DataFrame({"util_ratio": util, LABEL_COL: df[LABEL_COL]})
    tmp["util_bin"] = pd.cut(tmp["util_ratio"], bins=[-1, 0, 0.3, 0.6, 0.9, 1.2, 3], include_lowest=True)
    rates = tmp.groupby("util_bin", observed=True)[LABEL_COL].mean()
    fig, ax = plt.subplots(figsize=(8, 4))
    rates.plot(kind="bar", ax=ax, color="#2A6F97")
    ax.set_ylabel("Odsetek defaultów")
    ax.set_xlabel("Przedział wykorzystania limitu (BILL_AMT1 / LIMIT_BAL)")
    ax.set_title("Teza T2: util_ratio a ryzyko defaultu")
    ax.set_ylim(0, max(0.5, rates.max() * 1.2))
    fig.tight_layout()
    path = FIGURES / "04_util_ratio.png"
    fig.savefig(path, dpi=140)
    plt.close(fig)
    return path


def fig_metrics():
    metrics_path = MODELS / "metrics_rf.json"
    if not metrics_path.exists():
        return None
    m = json.loads(metrics_path.read_text(encoding="utf-8"))
    keys = ["auc", "accuracy", "f1", "precision", "recall"]
    vals = [m[k] for k in keys]
    fig, ax = plt.subplots(figsize=(7, 4))
    bars = ax.bar(keys, vals, color="#1B4332")
    ax.axhline(0.70, color="#C1121F", linestyle="--", label="Próg tezy T3 (AUC>0.70)")
    ax.set_ylim(0, 1)
    ax.set_title(f"MLlib RandomForest — metryki (AUC={m['auc']:.3f})")
    ax.legend()
    for b, v in zip(bars, vals):
        ax.text(b.get_x() + b.get_width() / 2, v + 0.02, f"{v:.3f}", ha="center", fontsize=9)
    fig.tight_layout()
    path = FIGURES / "05_ml_metrics.png"
    fig.savefig(path, dpi=140)
    plt.close(fig)

    if m.get("top_features"):
        top = m["top_features"][:8]
        fig, ax = plt.subplots(figsize=(8, 4))
        ax.barh([t["feature"] for t in top][::-1], [t["importance"] for t in top][::-1], color="#2A6F97")
        ax.set_title("Ważność cech — RandomForest (MLlib)")
        ax.set_xlabel("Importance")
        fig.tight_layout()
        path2 = FIGURES / "06_feature_importance.png"
        fig.savefig(path2, dpi=140)
        plt.close(fig)
        return path, path2
    return path


def fig_corr(df: pd.DataFrame):
    cols = ["LIMIT_BAL", "AGE", "PAY_0", "PAY_2", "BILL_AMT1", "PAY_AMT1", LABEL_COL]
    corr = df[cols].corr()
    fig, ax = plt.subplots(figsize=(7, 5))
    sns.heatmap(corr, annot=True, fmt=".2f", cmap="RdBu_r", center=0, ax=ax)
    ax.set_title("Macierz korelacji (wybrane cechy)")
    fig.tight_layout()
    path = FIGURES / "07_correlation.png"
    fig.savefig(path, dpi=140)
    plt.close(fig)
    return path


def main():
    FIGURES.mkdir(parents=True, exist_ok=True)
    df = load_df()
    paths = [
        fig_class_balance(df),
        fig_age_limit(df),
        fig_pay_status(df),
        fig_util_ratio(df),
        fig_corr(df),
    ]
    ml = fig_metrics()
    if ml:
        if isinstance(ml, tuple):
            paths.extend(ml)
        else:
            paths.append(ml)
    print("Zapisano wykresy:")
    for p in paths:
        if p:
            print(f"  {p}")


if __name__ == "__main__":
    main()
