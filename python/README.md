# VERNIER Python bindings

`pyvernier` exposes the VERNIER C++ library to Python through
[nanobind](https://github.com/wjakob/nanobind) (vendored in `3rdparty/nanobind`).
It covers the common workflow: build a pattern layout, render an image of it,
then detect the pattern and read back its pose. Every C++ example has a Python
port in `examples/` (see [Examples](#examples)).

## Building

Built as part of the normal CMake build, enabled by default
(`-DBUILD_PYTHON_BINDINGS=ON`). Needs the Python 3.8+ development headers
(`python3-dev`); nanobind requires C++17 (only for this target, the rest of the
library stays C++14).

```bash
mkdir -p build && cd build
cmake ..
make pyvernier
```

The module (`pyvernier.cpython-*.so`) lands in `build/python/`. CMake copies the
examples, the tests and the images they read next to it, flattening the source
layout below, so everything runs from `build/python` with no `PYTHONPATH` to set.

## Layout

```
python/
├── CMakeLists.txt
├── README.md
├── vernier_python.cpp   the nanobind module
├── examples/            one file per C++ example, plus the CUDA ones
└── test/                unittest suite
```

### With the CUDA backend

Add `-DUSE_CUDA=ON` to get the GPU path compiled in (needs the CUDA toolkit and
cuFFT). It is selected at runtime, so a build with CUDA still runs on machines
without a device.

```bash
cmake -DUSE_CUDA=ON ..
make pyvernier
```

## Usage

Images are 2-D `float64` NumPy arrays (mapping onto `Eigen::ArrayXXd`).

```python
import pyvernier as vernier

# Render a periodic pattern at a known pose ...
layout = vernier.PeriodicPatternLayout(15.0, 31, 31)   # period, nRows, nCols
image = layout.renderOrthographicProjection(
    vernier.Pose(6.0, 3.0, 0.2, 2.0), 512, 512)         # x, y, alpha, pixelSize

# ... and recover it.
detector = vernier.PeriodicPatternDetector(15.0)
detector.compute(image)
if detector.patternFound():
    print(detector.get2DPose())
```

### Running on the GPU

The phase-retrieval stage (`PatternPhase`) — FFT, spectrum filtering, peak
search, inverse FFTs, i.e. where a detection spends its time — has a CUDA
implementation (cuFFT + custom kernels), picked at runtime. Nothing else in the
API changes.

```python
if vernier.cudaAvailable():          # built with CUDA *and* a device is present
    detector.setBackend(vernier.Backend.CUDA)
```

`setBackend` raises `VernierError` when CUDA is requested but the library was
built without it or no device is visible, so `cudaAvailable()` is the cheap way
to fall back to the CPU. `detector.setBackend(...)` is a shortcut for
`detector.getPatternPhase().setBackend(...)`; a bare `PatternPhase` (see
`examples/bench.py`) takes the same call.

Both backends compute in double precision and agree to within rounding.

## Exposed API

- `Pose` — `x, y, z, alpha, beta, gamma` (2D and 3D constructors).
- `PatternDetector` — `compute(image)`, `patternFound()`, `patternCount()`,
  `get2DPose()`, `get3DPose()`, `getAll3DPoses()`, `draw(image)`,
  `showControlImages()`.
- `PeriodicPatternDetector`, `MegarenaPatternDetector`,
  `HPCodePatternDetector`, `StampPatternDetector` — the above plus
  `setSigma()`, `setCropFactor()`, `setBackend(backend)`, `getBackend()`,
  `getPatternPhase()`; the two marker detectors also expose `markers`, a
  `{id: Pose}` dict.
- `PatternPhase` — `compute(image)`, `resize(rows, cols)`, `setBackend(backend)`,
  `getBackend()`, `peaksFound()`, `getPlane1/2()`, `getUnwrappedPhase1/2()`,
  `showControlImages()`.
- `PhasePlane` — `a, b, c`, `getPosition(physicalPeriod)`.
- `Backend` — `Backend.CPU`, `Backend.CUDA`; `cudaAvailable()` at module level.
- `Layout.loadFromJSON(filename)` — builds the layout described by a JSON file.
- `PatternLayout` — `renderOrthographicProjection(pose, rows, cols)`,
  `saveToPNG()`, `saveToSVG()`, `saveToJSON()`; `PeriodicPatternLayout(period,
  nRows, nCols)` builds one directly (`nRows`/`nCols` must be odd).
  `pngCellSize` (pixels per half period, default 1) and `pngCornerRadius`
  (0.0 to 0.5 of a cell, default 0.0) set the PNG resolution and round the
  corners of the dots, merging adjacent dots into smooth groups.
- `readImage(filename)`, `saveImage(filename, image)`, `showImage(name, image)`,
  `waitKey(delay)` — image I/O and display, so the examples need no extra
  Python package.
- `VernierError` — exception raised for library errors.

### Images

Grayscale images are 2-D `float64` arrays with intensities in `[0, 1]`, which is
what `readImage()` returns and what the detectors expect. `draw()` differs from
its C++ counterpart: instead of annotating a `cv::Mat` in place it returns an
annotated `(rows, cols, 3)` `uint8` RGB copy, which `showImage()` and
`saveImage()` also accept.

## Examples

`python/examples/` holds a port of every C++ example in the top-level
`examples/`, under the same name, plus one CUDA-specific example. Run them from
the build directory, where the module and the images live:

```bash
cd build/python
python3 detectingPeriodicPattern.py  # render a pattern, detect it, print the pose
python3 analysingImage.py            # spectrum analysis and phase planes
python3 detectingMegarenaPattern.py  # pose of a megarena pattern
python3 detectingMegarenaPattern3D.py
python3 detectingHPCodePattern.py    # pose of HP code markers
python3 detectingStampPattern.py     # pose of stamp markers
python3 generatingPatternLayout.py   # JSON layout -> PNG and SVG
python3 renderingPatternImage.py     # render a layout at a given pose
python3 usingCudaBackend.py          # the same detection on the CPU and on the GPU
python3 bench.py --size 2048 --iters 50   # CPU vs CUDA timings
```

The ports print exactly what the C++ examples print. Where the C++ version ends
with `imshow` / `waitKey`, the Python one only opens windows when passed
`--show`, so it stays usable on a headless machine. The two CUDA examples fall
back to the CPU path with a message when no GPU backend is available, so they
run anywhere.

## Tests

```bash
cd build/python
python3 -m unittest test_vernier -v
```

The CUDA tests skip themselves when the GPU backend is not available.
