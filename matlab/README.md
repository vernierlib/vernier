# VERNIER Matlab bindings

The `+vernier` package exposes the pattern detectors of the VERNIER library to
Matlab. It calls the [C API](../c/README.md) through `loadlibrary`, so there is
nothing to compile for Matlab itself: the bindings are plain `.m` files, and the
only binary they need is `vernier_c`, the shared library of the C API.

It covers detection of every kind of pattern (periodic, megarena, bitmap, stamp
and HP code): build a detector, hand it an image, read back the pose of the
pattern. The layouts, which describe and render patterns and export them for
manufacturing, are only available through the C++ classes and the Python
bindings.

## Building

Only the C API has to be built (`-DBUILD_C_API=ON`, the default):

```bash
mkdir -p build && cd build
cmake ..
make vernier_matlab
```

`build/matlab/` then holds the `+vernier` package, a copy of the C API library
and its header, `test_vernier.m`, `vernier_example.m` and the test images. Add
that directory to the Matlab path:

```matlab
addpath('/path/to/vernier/build/matlab')
```

`make install` copies the package and the header to `<prefix>/matlab`, next to
the library in `<prefix>/lib`.

By default the package loads the library and the header sitting beside it. To
use another build, load it explicitly before the first call:

```matlab
vernier.Library.load('/path/to/libvernier_c.so', '/path/to/vernier.h')
```

## Usage

```matlab
% A camera image, straight from imread
image = imread('megarenaPatternImage_12bits_9um.jpg');

detector = vernier.MegarenaPatternDetector(9, 12);   % physical period (um), code size (bits)
detector.compute(image);
if detector.patternFound()
    disp(detector.get2DPose())
end
```

Images are 2-D matrices. As with the C++ interface, `double` and `single`
images are expected in [0, 1], `uint8` and `uint16` images are scaled to
[0, 1] and RGB images are converted to grayscale, so the output of `imread` can
be passed directly.

## Exposed API

- `vernier.Pose` — value class with `x, y, z, alpha, beta, gamma, pixelSize,
  is3D` (2D and 3D constructors), `toString()`,
  `getCameraToPatternTransformationMatrix()`,
  `getPatternToCameraTransformationMatrix()`.
- `vernier.PatternDetector` — base class of the detectors: `compute(image)`,
  `patternFound()`, `patternCount()`, `get2DPose()`, `get3DPose()`,
  `getAll3DPoses()`, `setBackend(backend)`, `getBackend()`, `getClassname()`,
  `toString()`, `getDescription()`, `getAuthor()`, `getDate()`, `getUnit()`.
  - `vernier.PeriodicPatternDetector(physicalPeriod)` — adds `setSigma`,
    `setCropFactor`, `setMinFrequency`, `setMaxFrequency`, `setMinPeakPower`,
    `setSmoothingKernelSize`, `setPhysicalPeriod`, `getPhysicalPeriod`,
    `get3DPosePerspective(focalLength)`.
  - `vernier.MegarenaPatternDetector(physicalPeriod, codeSize)`.
  - `vernier.BitmapPatternDetector(physicalPeriod, filename)` — the bitmap is
    read from a PNG file.
  - `vernier.StampPatternDetector(physicalPeriod, filename, snapshotSize)` —
    adds `addBitmap(filename)`.
  - `vernier.HPCodePatternDetector(physicalPeriod[, numberHalfPeriods, snapshotSize])`.
  - Stamp and HP code detectors locate every marker of an image:
    `getMarkerIds()` lists their ids, to be passed to `patternFound(id)` and
    `get2DPose(id)`, and `getMarkers()` returns a `containers.Map` from id to
    pose.
- `vernier.Detector.newInstance(classname[, backend])`,
  `vernier.Detector.loadFromJSON(filename[, backend])` — factories returning
  the class matching the kind of pattern.
- `vernier.cudaAvailable()` — true if the `'CUDA'` backend can be selected.
- `vernier.Library` — `load()`, `unload()`, `version()`; used by the classes
  above, and to point them at another build of the library.

Errors from the library are raised with the identifier `vernier:error`.

Detectors are handle objects: the underlying detector is freed when the last
reference is cleared. Clear them before `vernier.Library.unload()`.

## CUDA backend

When the library is built with `-DUSE_CUDA=ON`, the phase computation can run on
a CUDA device. The backend is selected at runtime, as the string `'CPU'` or
`'CUDA'`, and the CPU stays the default:

```matlab
detector = vernier.MegarenaPatternDetector(9, 12);
if vernier.cudaAvailable()          % built with CUDA and a device is present
    detector.setBackend('CUDA');
end
```

The factories take the backend as an optional last argument, e.g.
`vernier.Detector.loadFromJSON('pattern.json', 'CUDA')`. Selecting CUDA without
CUDA support or without a device raises a `vernier:error` and leaves the
detector on its current backend.

## Tests

```bash
cd build
ctest -R TestMatlabBindings --output-on-failure
```

or, from `build/matlab`, `matlab -batch test_vernier`. The GitHub workflow runs
them with Matlab on Linux, Windows and macOS (in the `matlab` job).

The tests detect patterns in the images of `data/`, rendered from the C++
layouts at known poses (see [../c/data/README.md](../c/data/README.md)), and in
the camera images of the examples.

## Troubleshooting

- `loadlibrary` parses `vernier.h` when loading the library, which needs a
  supported C compiler on Windows. Alternatively, generate a prototype file
  once on a machine that has one (`loadlibrary(..., 'mfilename', 'vernier_proto')`)
  and ship it alongside the package.
- On Windows the OpenCV DLLs must be on the `PATH` seen by Matlab for
  `vernier_c.dll` to load.
- On Linux, Matlab loads its own copies of common libraries (libstdc++,
  libcurl, libxml2, ...) before the system ones, and `loadlibrary` fails with
  a message such as `version 'CURL_OPENSSL_4' not found` when the library or
  a dependency of OpenCV needs the system version. Preload the system
  libraries when starting Matlab, for instance every dependency of
  `libvernier_c.so` that Matlab also ships:

  ```bash
  root=/usr/local/MATLAB/R2026b
  preload=""
  for lib in $(ldd libvernier_c.so | awk '/=> \//{print $3}'); do
    name=$(basename "$lib")
    if [ -e "$root/bin/glnxa64/$name" ] || [ -e "$root/sys/os/glnxa64/$name" ]; then
      preload="$preload${preload:+:}$lib"
    fi
  done
  LD_PRELOAD="$preload" matlab
  ```

  The `matlab` job of the GitHub workflow does the same.
- On macOS, Matlab aborts with `OMP: Error #15` when `vernier_c` or the
  Homebrew OpenCV behind it brings a second OpenMP runtime. Start Matlab with
  `KMP_DUPLICATE_LIB_OK=TRUE` in its environment, as the workflow does, or
  build OpenCV and the library without OpenMP.
- Matlab ships its own OpenCV libraries, which may clash with the system ones
  the C API is linked against.
