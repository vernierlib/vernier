#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Python port of examples/bench.cpp: times the phase-retrieval stage.

It renders the same image as the C++ version, feeds it to `PatternPhase`
repeatedly and reports the mean and best iteration time for each backend. Useful
to check what the CUDA backend buys on a given machine and image size (the GPU
path pays a fixed host/device transfer cost, so it pays off on larger images).

    cd build/python
    python3 bench.py                          # both backends, 512x512
    python3 bench.py --size 2048 --iters 50
    python3 bench.py --backend cuda
"""

import argparse
import time

import pyvernier as vernier

PHYSICAL_PERIOD = 15.0  # µm
PERIODS_PER_IMAGE = 16


def synthetic_image(size):
    """Renders the benchmark image, the same one as examples/bench.cpp.

    Rather than hand-rolling a cosine, this asks the library for a real periodic
    pattern seen slightly rotated by a virtual camera. The pixel size is chosen
    so that the image always spans PERIODS_PER_IMAGE periods, whatever `size` is.
    """
    layout = vernier.PeriodicPatternLayout(PHYSICAL_PERIOD, 33, 33)
    pixel_size = PHYSICAL_PERIOD * PERIODS_PER_IMAGE / size
    pose = vernier.Pose(0.0, 0.0, 0.1, pixel_size)  # x, y, alpha, pixelSize
    return layout.renderOrthographicProjection(pose, size, size)


def bench(image, backend, iters):
    """Returns (mean_ms, best_ms) over `iters` runs of the pipeline."""
    size = image.shape[0]
    pattern_phase = vernier.PatternPhase(size, size)
    pattern_phase.setBackend(backend)

    # Warm-up: primes the FFT plans and the device buffers, which would
    # otherwise be charged to the first iteration.
    pattern_phase.compute(image)

    times = []
    for _ in range(iters):
        start = time.perf_counter()
        pattern_phase.compute(image)
        times.append((time.perf_counter() - start) * 1e3)
    return sum(times) / len(times), min(times)


parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--size", type=int, default=512, help="image side, in pixels")
parser.add_argument("--iters", type=int, default=20, help="timed iterations per backend")
parser.add_argument("--backend", choices=("cpu", "cuda", "both"), default="both")
args = parser.parse_args()

backends = {"cpu": vernier.Backend.CPU, "cuda": vernier.Backend.CUDA}
selected = ["cpu", "cuda"] if args.backend == "both" else [args.backend]

if "cuda" in selected and not vernier.cudaAvailable():
    if args.backend == "cuda":
        raise SystemExit("CUDA backend unavailable: build with -DUSE_CUDA=ON "
                         "and run on a machine with a CUDA device.")
    print("CUDA backend unavailable, benchmarking the CPU path only.")
    selected.remove("cuda")

image = synthetic_image(args.size)
results = {}
for name in selected:
    mean_ms, best_ms = bench(image, backends[name], args.iters)
    results[name] = best_ms
    print("backend=python-%-4s size=%dx%d iters=%d mean=%.2fms best=%.2fms"
          % (name, args.size, args.size, args.iters, mean_ms, best_ms))

if len(results) == 2:
    print("speedup (best): x%.2f" % (results["cpu"] / results["cuda"]))
