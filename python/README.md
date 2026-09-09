# VERNIER Python bindings

`pyvernier` exposes the VERNIER C++ library to Python through
[nanobind](https://github.com/wjakob/nanobind) (vendored in `3rdparty/nanobind`).
It covers the common workflow: build a pattern layout, render an image of it,
then detect the pattern and read back its pose.

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

The module (`pyvernier.cpython-*.so`) lands in `build/python/`, next to the
examples and `test_vernier.py`.

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
`bench_cuda.py`) takes the same call.

Both backends compute in double precision and agree to within rounding.

## Exposed API

- `Pose` — `x, y, z, alpha, beta, gamma` (2D and 3D constructors).
- `PeriodicPatternDetector`, `MegarenaPatternDetector` — `compute(image)`,
  `patternFound()`, `get2DPose()`, `get3DPose()`, `setBackend(backend)`,
  `getBackend()`, `getPatternPhase()`.
- `PatternPhase` — `compute(image)`, `resize(rows, cols)`, `setBackend(backend)`,
  `getBackend()`, `peaksFound()`, `getUnwrappedPhase1/2()`.
- `Backend` — `Backend.CPU`, `Backend.CUDA`; `cudaAvailable()` at module level.
- `PeriodicPatternLayout` — `renderOrthographicProjection(pose, rows, cols)`.
- `VernierError` — exception raised for library errors.

## Examples

Run from the build directory, where the module lives:

```bash
cd build/python
python3 example.py           # render a pattern, detect it, print the pose
python3 example_cuda.py      # the same detection on the CPU and on the GPU
python3 bench_cuda.py --size 2048 --iters 50   # CPU vs CUDA timings
```

Both CUDA examples degrade to the CPU path with a message when no GPU backend
is available, so they run anywhere.

## Tests

```bash
cd build/python
python3 -m unittest test_vernier -v
```

The CUDA tests skip themselves when the GPU backend is not available.
