"""Figures of the peak search comparison, from scored.csv, results.csv and
truth.csv. Run from this directory after analyse.py (see run.sh)."""

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from PIL import Image

MAXIMUM = "#eb6834"
RING = "#2a78d6"
TRUTH = "#1baf7a"
INK = "#1f1f1e"
MUTED = "#6b6a63"
GRID = "#e4e3dd"

plt.rcParams.update(
    {
        "font.size": 9,
        "axes.edgecolor": MUTED,
        "axes.labelcolor": INK,
        "axes.titlesize": 9.5,
        "axes.titleweight": "bold",
        "axes.titlecolor": INK,
        "xtick.color": MUTED,
        "ytick.color": MUTED,
        "axes.grid": True,
        "grid.color": GRID,
        "grid.linewidth": 0.6,
        "axes.spines.top": False,
        "axes.spines.right": False,
        "legend.frameon": False,
        "savefig.dpi": 200,
        "savefig.bbox": "tight",
    }
)

WIDTH, HEIGHT = 1280, 720
scored = pd.read_csv("scored.csv")
results = pd.read_csv("results.csv")
truth = pd.read_csv("truth.csv")
SEARCHES = [("maximum", "strongest bin (today)", MAXIMUM), ("ring", "ring search", RING)]


def track_bars(ax, labels, values, total, colours):
    """Horizontal bars against a grey track of the total, labelled with the count."""
    for y, (value, colour) in enumerate(zip(values, colours)):
        ax.barh(y, total, color=GRID, height=0.62, zorder=1)
        ax.barh(y, value, color=colour, height=0.62, zorder=2)
        text = f"{value} of {total}  ({100 * value / total:.0f} %)"
        if value > 0.7 * total:
            ax.text(value - total * 0.01, y, text, va="center", ha="right", fontsize=8.5,
                    color="white", fontweight="bold")
        else:
            ax.text(value + total * 0.01, y, text, va="center", fontsize=8.5, color=INK)
    ax.set_yticks(range(len(labels)), labels, fontsize=8.5)
    ax.invert_yaxis()
    ax.set_xlim(0, total)
    ax.set_xticks([])
    ax.grid(False)
    for side in ("left", "bottom"):
        ax.spines[side].set_visible(False)
    ax.tick_params(left=False)


# Every frame: right carrier pair, then right position.
def overall():
    total = len(scored)
    fig, axes = plt.subplots(2, 1, figsize=(7.2, 2.6))
    for ax, key, title in ((axes[0], "pair", "Frames whose two carriers were found"),
                           (axes[1], "position", "Frames decoded at the right position")):
        track_bars(ax, [label for _, label, _ in SEARCHES],
                   [int(scored[f"{s}_{key}"].sum()) for s, _, _ in SEARCHES], total,
                   [c for _, _, c in SEARCHES])
        ax.set_title(title, loc="left")
    fig.tight_layout(pad=0.4, h_pad=1.2)
    fig.savefig("figures/overall.png")


def grouped(ax, groups, counts, total):
    width = 0.8 / len(SEARCHES)
    x = np.arange(len(groups))
    for i, ((_, label, colour), values) in enumerate(zip(SEARCHES, counts)):
        positions = x - 0.4 + width * (i + 0.5)
        ax.bar(positions, values, width * 0.92, color=colour, label=label, zorder=2)
        for position, value in zip(positions, values):
            ax.text(position, value + total * 0.015, f"{value}", ha="center", va="bottom",
                    fontsize=7, color=MUTED)
    ax.set_xticks(x, groups)
    ax.set_ylim(0, total * 1.12)
    ax.axhline(total, color=MUTED, linewidth=0.8, linestyle="--", zorder=1)
    ax.grid(axis="x", visible=False)


# The clean test views by tilt.
def tilt():
    clean = scored[(scored.set == "test") & (scored.condition == "Clean")]
    tilts = [0, 15, 30, 50]
    fig, axes = plt.subplots(1, 2, figsize=(7.4, 2.5))
    for ax, key, title in ((axes[0], "pair", "Right carrier pair, of 24 views"),
                           (axes[1], "position", "Right position, of 24 views")):
        counts = [[int(clean[clean.tilt == t][f"{s}_{key}"].sum()) for t in tilts] for s, _, _ in SEARCHES]
        grouped(ax, [f"{t}°" for t in tilts], counts, 24)
        ax.set_title(title)
        ax.set_xlabel("tilt")
    fig.tight_layout(pad=0.4, w_pad=2.0, rect=(0, 0.08, 1, 1))
    fig.legend(*axes[0].get_legend_handles_labels(), loc="lower center", ncol=2, fontsize=7.5)
    fig.savefig("figures/tilt.png")


# Every view by what was added to the picture.
def conditions():
    names = [("Clean", "clean"), ("Noise", "noise"), ("Shadows", "shadows"), ("Blind", "blind")]
    fig, axes = plt.subplots(1, 2, figsize=(7.4, 2.6))
    for ax, key, title in ((axes[0], "pair", "Right carrier pair, of 176 frames"),
                           (axes[1], "position", "Right position, of 176 frames")):
        counts = [[int(scored[scored.condition == c][f"{s}_{key}"].sum()) for c, _ in names]
                  for s, _, _ in SEARCHES]
        grouped(ax, [label for _, label in names], counts, 176)
        ax.set_title(title)
    fig.tight_layout(pad=0.4, w_pad=2.0, rect=(0, 0.08, 1, 1))
    fig.legend(*axes[0].get_legend_handles_labels(), loc="lower center", ncol=2, fontsize=7.5)
    fig.savefig("figures/conditions.png")


# What each search picked, on a square-on and a tilted picture.
def spectrum():
    cases = [("view_square.png", "megarena-3.54_test_04_Clean.png", "Square on (260 mm, 0°)"),
             ("view_tilted.png", "megarena-3.54_test_05_Clean.png", "Tilted (260 mm, 15°)")]
    fig, axes = plt.subplots(2, 2, figsize=(7.2, 5.6), gridspec_kw={"height_ratios": [1, 1.55]})
    for column, (picture, file, title) in enumerate(cases):
        image = np.asarray(Image.open(f"figures/{picture}").convert("L"), dtype=float) / 255
        axes[0, column].imshow(image, cmap="gray", vmin=0, vmax=1)
        axes[0, column].set_title(title)
        axes[0, column].axis("off")

        magnitude = np.abs(np.fft.fftshift(np.fft.fft2(image)))
        reach = 0.16
        fx = np.fft.fftshift(np.fft.fftfreq(WIDTH))
        fy = np.fft.fftshift(np.fft.fftfreq(HEIGHT))
        cx, cy = np.abs(fx) <= reach, np.abs(fy) <= reach
        shown = np.log1p(magnitude[np.ix_(cy, cx)])
        ax = axes[1, column]
        ax.imshow(shown, cmap="Greys", origin="lower", vmin=np.percentile(shown, 40),
                  vmax=np.percentile(shown, 99.9), extent=[fx[cx][0], fx[cx][-1], fy[cy][0], fy[cy][-1]],
                  aspect="equal")
        ax.grid(False)

        t = truth[truth.file == file].iloc[0]
        for i, k in enumerate(("k1", "k2")):
            for sign in (1, -1):
                ax.scatter(sign * t[f"truth_{k}x"] / (2 * np.pi), sign * t[f"truth_{k}y"] / (2 * np.pi),
                           marker="o", s=120, facecolors="none", edgecolors=TRUTH, linewidths=1.4, zorder=3,
                           label="true carriers" if (i, sign) == (0, 1) else None)
        for search, label, colour in SEARCHES:
            r = results[(results.file == file) & (results.search == search)].iloc[0]
            style, size = ("x", 45) if search == "maximum" else ("+", 70)
            for i in (1, 2):
                for sign in (1, -1):
                    ax.scatter(sign * (r[f"peak{i}_col"] - WIDTH // 2) / WIDTH,
                               sign * (r[f"peak{i}_row"] - HEIGHT // 2) / HEIGHT, marker=style, s=size,
                               color=colour, linewidths=1.4, zorder=4,
                               label=label if (i, sign) == (1, 1) else None)
        ax.set_xlabel("cycles per pixel, x")
        if column == 0:
            ax.set_ylabel("cycles per pixel, y")
        else:
            ax.tick_params(labelleft=False)
    axes[1, 1].legend(loc="upper right", fontsize=7, handletextpad=0.3, borderaxespad=0.2)
    fig.tight_layout(pad=0.4, h_pad=0.8)
    fig.savefig("figures/spectrum.png")


overall()
spectrum()
tilt()
conditions()
