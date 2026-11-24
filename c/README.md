# VERNIER C API

`vernier_c` is a shared library exposing the pattern detectors of the VERNIER
library through a flat C interface (`vernier.h`), so that they can be used from
any language able to load a C shared library — C, C#, LabVIEW, Julia, Rust,
Python `ctypes`, Matlab — without depending on a C++ ABI.

It covers detection: build a detector, hand it an image, read back the pose of
the pattern. The layouts, which describe and render patterns and export them for
manufacturing, are only available through the C++ classes and the Python
bindings.

C++ users should keep using the C++ classes directly, and Python users the
`pyvernier` module: both are richer than this interface.

## Building

Built as part of the normal CMake build (`-DBUILD_C_API=ON`, the default):

```bash
mkdir -p build && cd build
cmake ..
make vernier_c
```

`libvernier_c.so` (`.dylib` on macOS, `vernier_c.dll` on Windows) lands in
`build/c/`. `make install` copies it to `<prefix>/lib` and the header to
`<prefix>/include/VernierLibrary`.

## Usage

```c
#include <vernier.h>

VernierDetector * detector = vernierDetectorNewMegarena(9.0, 12);
VernierPose pose;
int found = 0;

if (detector == NULL) {
    fprintf(stderr, "%s\n", vernierLastError());
    return 1;
}
/* image is rows * cols doubles in [0, 1], stored column by column */
vernierDetectorCompute(detector, image, rows, cols);
vernierDetectorPatternFound(detector, -1, &found);
if (found) {
    vernierDetectorGet2DPose(detector, -1, &pose);
    printf("x = %f, y = %f, alpha = %f\n", pose.x, pose.y, pose.alpha);
}
vernierDetectorFree(detector);
```

## Conventions

- **Handles.** `VernierDetector` is opaque. Create one with the
  `vernierDetectorNew*` or `vernierDetectorLoadFromJSON` functions, which
  return `NULL` on failure, and release it with `vernierDetectorFree`; freeing
  `NULL` is a no-op.
- **Errors.** Every other function returns a `VernierStatus`: `VERNIER_OK`,
  `VERNIER_ERROR` (the library refused the call, for instance an unknown
  pattern name, or a parameter that does not apply to this kind of detector) or
  `VERNIER_INVALID_ARGUMENT` (a null pointer or a buffer too small). On failure
  the detector is left unchanged and `vernierLastError()` describes the problem.
  Nothing throws across the boundary.
- **Images.** Arrays of `rows * cols` doubles in [0, 1], stored column by
  column: element (row, col) is at `image[row + col * rows]`. This is the
  layout of Eigen, Matlab, Fortran and Julia; callers holding row-major images
  (C, NumPy) should transpose. The caller owns the memory in both directions:
  the library never allocates an array on the caller's behalf.
- **Strings.** Copied into a caller-provided buffer. A buffer too small gets a
  truncated, null-terminated string and `VERNIER_INVALID_ARGUMENT`.
- **Poses.** Plain `VernierPose` structs, with `is3D` telling whether `z`,
  `beta` and `gamma` are meaningful. Transformation matrices are written as 16
  doubles, column by column.
- **Threads.** The error message is kept per thread. The detectors themselves
  are not thread-safe: do not use one handle from two threads at once.

## Exposed API

- **Library** — `vernierVersion()`, `vernierCudaAvailable()`,
  `vernierLastError()`.
- **Poses** — `vernierPoseToString()`, `vernierPoseCameraToPatternMatrix()`,
  `vernierPosePatternToCameraMatrix()`.
- **Detectors** — `vernierDetectorNew()` (by class name),
  `vernierDetectorLoadFromJSON()`, `vernierDetectorNewPeriodic()`,
  `vernierDetectorNewMegarena()`, `vernierDetectorFree()`,
  `vernierDetectorCompute()`, `vernierDetectorPatternFound()`,
  `vernierDetectorPatternCount()`, `vernierDetectorGet2DPose()`,
  `vernierDetectorGet3DPose()`, `vernierDetectorGetAll3DPoses()`,
  `vernierDetectorGet3DPosePerspective()`, `vernierDetectorSetBackend()`,
  `vernierDetectorGetBackend()`, the periodic parameters
  (`SetPhysicalPeriod`, `GetPhysicalPeriod`, `SetSigma`, `SetCropFactor`,
  `SetMaxFrequency`, `SetMinFrequency`, `SetMinPeakPower`,
  `SetSmoothingKernelSize`) and the descriptions (`GetClassname`, `ToString`,
  `GetDescription`, `GetAuthor`, `GetDate`, `GetUnit`).

Only these functions are exported: the C++ internals are hidden, so they cannot
be bound to by mistake, nor clash with the OpenCV or Eigen an application brings
along.

## Tests

`test_vernier_c.c` is deliberately compiled as C, so it also checks that the
header is usable from C. It detects patterns in the images of `data/`, rendered
from the C++ layouts at known poses (see `data/README.md`):

```bash
cd build
ctest -R TestCApi --output-on-failure
```
