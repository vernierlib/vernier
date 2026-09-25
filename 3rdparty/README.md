# 3rd Party

Third party code redistributed with the Vernier Library. Each component keeps its
own licence file in its own directory; the terms below apply to that component
only, not to Vernier itself, which is GPL-3.0.

| Component | Version | Licence | Licence file |
| --- | --- | --- | --- |
| [Eigen](http://eigen.tuxfamily.org) | 3.3.9 | MPL-2.0, with LGPL-2.1 and BSD-3-Clause parts | [Eigen/COPYING.README](Eigen/COPYING.README) |
| [eigen-matio (MATio)](https://github.com/tesch1/eigen-matio) | 1.0.1 | MPL-2.0 | [eigen-matio/license.txt](eigen-matio/license.txt) |
| [FFTW](http://www.fftw.org/) | 3.3.5 | GPL-2.0-or-later | [fftw3/COPYING](fftw3/COPYING) |
| [gdstk](https://github.com/heitzmann/gdstk) | 0.9.59 | BSL-1.0 | [gdstk/LICENSE](gdstk/LICENSE) |
| [MAT File I/O Library (matio)](https://sourceforge.net/projects/matio/) | 1.5.21 | BSD-2-Clause | [matio/COPYING](matio/COPYING) |
| [Pathed](https://code.google.com/archive/p/pathed/) (Windows only) | 0.8 | MIT | [pathed/LICENSE](pathed/LICENSE) |
| [RapidJSON](https://rapidjson.org/) | 1.1.0 | MIT, with a BSD-3-Clause part | [rapidjson/license.txt](rapidjson/license.txt) |

[OpenCV](http://opencv.org/) 4.6.0 or higher is required as well, but it is not
redistributed here: it is found on the system at configure time and stays under
its own licence (Apache-2.0 from 4.5.0 on, BSD-3-Clause before).

## Notes on individual components

### Eigen

`COPYING.README` is the upstream summary of how the licences apply. Most of Eigen
is MPL-2.0; `src/SparseCholesky/SimplicialCholesky_impl.h` and
`src/OrderingMethods/Amd.h` carry LGPL-2.1 code from SuiteSparse, and the LAPACKE,
BLAS and MKL support headers carry BSD-3-Clause code from Intel. Defining
`EIGEN_MPL2_ONLY` makes including any LGPL part a compile error. Upstream's
`COPYING.MINPACK` is not reproduced because no Minpack code is bundled: it lives
in Eigen's `unsupported/` tree, which is not included here.

### RapidJSON

The bundled copy includes `msinttypes`, which is BSD-3-Clause; `license.txt`
covers both. Upstream's `bin/jsonchecker/` directory, the only part under the
non-free JSON licence, is not included here. `license.txt` is installed with the
headers, since `Common.hpp` includes `<rapidjson/document.hpp>` and the headers
therefore ship with the package.

### Pathed

`pathed.exe` is a prebuilt Windows binary, redistributed without its source. It is
the unmodified upstream 0.8 release, so its provenance can be checked:

| | |
| --- | --- |
| Upstream release | `pathed-08.zip`, published 2011-06-26 |
| SHA-1 of `pathed.exe` | `9f26c3304ef2acfe85e197479527066caebdc387` |
| Source revision | Mercurial tag `version-0.8`, `0c495198350d97fe5f916fb0d209f27b1b60c94b` |
| Source archive | <https://storage.googleapis.com/google-code-archive-source/v2/code.google.com/pathed/source-archive.zip> |

On Windows, the top-level `CMakeLists.txt` runs `pathed.exe -a <dir>` at configure
time for the bundled FFTW directory and for the OpenCV library directory. This
writes to the user's `PATH` in the registry, under `HKEY_CURRENT_USER`, so that the
DLLs are found at run time. It is a persistent change to the user's environment
made as a side effect of configuring the build.

To avoid it, delete the two `execute_process(... pathed.exe ...)` calls from the
top-level `CMakeLists.txt` and instead add these two directories to `PATH`
yourself, either in the environment used to run the programs or permanently
through *System Properties -> Environment Variables*:

  - `3rdparty/fftw3` in the source tree
  - the OpenCV library directory, the one CMake reports as `OpenCV_LIB_PATH`, plus
    the `bin` directory beside it if your OpenCV build keeps its DLLs there, for
    example `C:\opencv\build\x64\vc15\bin`
