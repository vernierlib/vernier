"""Scores the peak search comparison against the truth: whether each search
found the right carrier pair, and whether the megarena was decoded at the
right position. Reads truth.csv and results.csv, writes scored.csv and prints
the counts the report quotes. Run from this directory (see run.sh)."""

import collections
import csv
import math

truth = {r["file"]: r for r in csv.DictReader(open("truth.csv"))}
results = collections.defaultdict(dict)
for r in csv.DictReader(open("results.csv")):
    results[r["file"]][r["search"]] = r

SEARCHES = ("maximum", "ring")
# A code bit is three periods and the order-6 sequence 63 bits long, so the
# decoded position repeats every 189 periods.
CODE_PERIODS = 189
# The detector reports minus the board point under the picture's centre,
# offset by 15 periods on both axes: found on the clean square-on views, the
# same to a hundredth of a period on every board.
OFFSET_PERIODS = 15


def wrap(value, length):
    return (value + length / 2) % length - length / 2


def matches(found, true):
    tolerance = 0.2 * math.hypot(*true)
    return min(math.hypot(found[0] - true[0], found[1] - true[1]),
               math.hypot(found[0] + true[0], found[1] + true[1])) < tolerance


def floats(row, *keys):
    return [float(row[key]) for key in keys]


rows = []
for file, t in truth.items():
    pitch = float(t["pitch"])
    true1 = floats(t, "truth_k1x", "truth_k1y")
    true2 = floats(t, "truth_k2x", "truth_k2y")
    index = int(t["view"].split("_")[1])
    row = {key: t[key] for key in ("file", "board", "view", "condition", "pitch")}
    row["set"] = t["view"].split("_")[0]
    row["tilt"] = [0, 15, 30, 50][index % 4] if row["set"] == "test" else ""
    row["distance"] = [220, 260, 300, 350, 400, 450][index // 4] if row["set"] == "test" else ""
    for search in SEARCHES:
        r = results[file][search]
        # The fitted phase planes' gradients are the carriers demodulated.
        k1 = floats(r, "plane1_a", "plane1_b")
        k2 = floats(r, "plane2_a", "plane2_b")
        row[f"{search}_pair"] = (matches(k1, true1) and matches(k2, true2)) or (
            matches(k1, true2) and matches(k2, true1))
        length = CODE_PERIODS * pitch
        error_x = wrap(float(r["x"]) - (-float(t["centre_x"]) - OFFSET_PERIODS * pitch), length)
        error_y = wrap(float(r["y"]) - (-float(t["centre_y"]) - OFFSET_PERIODS * pitch), length)
        row[f"{search}_error"] = max(abs(error_x), abs(error_y)) / pitch
        row[f"{search}_position"] = row[f"{search}_error"] < 0.5
        row[f"{search}_found"] = r["found"] == "1"
    rows.append(row)

with open("scored.csv", "w", newline="") as out:
    writer = csv.DictWriter(out, list(rows[0].keys()))
    writer.writeheader()
    writer.writerows(rows)


def count(rs, key):
    return sum(bool(r[key]) for r in rs)


def line(label, rs):
    print(f"{label:24} {len(rs):4} | right pair: maximum {count(rs, 'maximum_pair'):4} ring {count(rs, 'ring_pair'):4}"
          f" | right position: maximum {count(rs, 'maximum_position'):4} ring {count(rs, 'ring_position'):4}")


line("all", rows)
for condition in ("Clean", "Noise", "Shadows", "Blind"):
    line(condition, [r for r in rows if r["condition"] == condition])
for board in sorted({r["board"] for r in rows}):
    line(board, [r for r in rows if r["board"] == board])
for tilt in (0, 15, 30, 50):
    line(f"clean test, {tilt}°", [r for r in rows if r["tilt"] == tilt and r["condition"] == "Clean"])
    line(f"every test, {tilt}°", [r for r in rows if r["tilt"] == tilt])
line("calibration views", [r for r in rows if r["set"] == "calibration"])
print("right position with the maximum only:", sum(r["maximum_position"] and not r["ring_position"] for r in rows))
print("right position with the ring only:   ", sum(r["ring_position"] and not r["maximum_position"] for r in rows))
both = [r for r in rows if r["maximum_pair"] and r["ring_pair"]]
print(f"both right pairs: {len(both)}; right position with both",
      sum(r["maximum_position"] and r["ring_position"] for r in both), "maximum only",
      sum(r["maximum_position"] and not r["ring_position"] for r in both), "ring only",
      sum(r["ring_position"] and not r["maximum_position"] for r in both))
print("right pair, wrong position: maximum", sum(r["maximum_pair"] and not r["maximum_position"] for r in rows),
      "ring", sum(r["ring_pair"] and not r["ring_position"] for r in rows))
print("pattern reported found: maximum", count(rows, "maximum_found"), "ring", count(rows, "ring_found"),
      "of", len(rows))
spread = collections.Counter()
for r in rows:
    for search in SEARCHES:
        e = r[f"{search}_error"]
        spread[(search, "under 0.5" if e < 0.5 else "0.5 to 3" if e < 3 else "3 or more")] += 1
print("position error, periods:", sorted(spread.items()))
