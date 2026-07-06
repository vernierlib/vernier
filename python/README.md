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

The module (`pyvernier.cpython-*.so`) lands in `build/python/`, next to
`example.py` and `test_vernier.py`.

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

## Exposed API

- `Pose` — `x, y, z, alpha, beta, gamma` (2D and 3D constructors).
- `PeriodicPatternDetector`, `MegarenaPatternDetector` — `compute(image)`,
  `patternFound()`, `get2DPose()`, `get3DPose()`.
- `PeriodicPatternLayout` — `renderOrthographicProjection(pose, rows, cols)`.
- `VernierError` — exception raised for library errors.

## Tests

```bash
cd build/python
python3 -m unittest test_vernier -v
```
