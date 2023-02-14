# Vernier Library 

[![Information](https://img.shields.io/badge/info-on_website-purple.svg)](https://projects.femto-st.fr/vernier/en)
[![Documentation](https://img.shields.io/badge/doc-on_website-blue.svg)](https://vernierlib.github.io/)
[![Contact](https://img.shields.io/badge/contact-form-green.svg)](https://projects.femto-st.fr/vernier/en/contact) 
[![GithHb](https://img.shields.io/badge/sources-on_github-orange.svg)](https://github.com/vernierlib) 

The Vernier Library is an open-source C++ library for pose measurement of calibrated patterns with subpixel resolutions.

The library gathers a collection of classes for detection and rendering different kind of calibrated patterns (periodic patterns, megarena patterns, QR-code-like and bitmap patterns).

## Documentation

The [documentation](https://vernierlib.github.io/) is automatically generated from the code using Doxygen.

## Compatibility

The Vernier library is cross-platform and required a C++14 compiler. Some platform/compiler combinations which have been tested are shown as follows.

* Visual C++ 2017/2019 on Windows (64-bit)
<!--- * GNU C++ 3.8.x on Cygwin -->
* Clang 14.0 on Mac OS X (64-bit)

Users can build and run the unit tests on their platform/compiler.

## Installation

The library uses following software as its dependencies:

* [CMake](https://cmake.org/) as a general build tool
* (optional) [Doxygen](http://www.doxygen.org) to build documentation
* (optional) [FFTW](https://www.fftw.org/) for improved performance 
* (optional) [OpenCV](https://opencv.org/) for compatibility and GUI

Many other packages are included inside the repositories ([see all dependencies](3rdparty/README.md)).

### Windows instructions

The package comes with all binaries files for Windows 64bits including OpenCV and FFTW ones.

To get everything running with Visual Studio (and other EDI):

1. Open the directory with CMake
2. Generate the project for your preferred EDI
3. Restart the computer for system path updating
4. Open the generated project

Alternatively, you can use VS Code.

### Linux instructions

With Linux, some dependencies must be installed first using homebrew:

	> sudo apt-get install cmake eigen libtool libmatio
	
Optionnally you may installed OpenCV and FFTW (recommanded):

	> sudo apt-get install opencv fftw

Then, open a terminal and go to the directory of the package

	> mkdir build
	> cd build
	> cmake ..
	> make

### OSX instructions

With OSX, some dependencies must be installed first using homebrew:

	% brew install autoconfig automake cmake eigen libtool libmatio
	
Optionnally you may installed OpenCV and FFTW (recommanded):

	% brew install opencv fftw

Then, open a terminal and go to the directory of the package

	% mkdir build
	% cd build
	% cmake ..
	% make

## Getting started

Run one of the demo files from the [examples page](https://vernierlib.github.io/examples.html)


## Licence

The Vernier Library is Free Software in the technical sense defined by the Free Software Foundation, and is distributed under the terms of the [GNU General Public License](LICENSE.txt). 

Non-free licenses are also available for companies that wish to use the Vernier library in their products but are unwilling to release their software under the GPL (which would require them to release source code and allow free redistribution). Contact us for more details: [vernier@femto-st.fr](mailto:vernier@femto-st.fr)

## Acknowledgements

We kindly ask you to acknowledge this library in any program or publication in which you use it. 

For general publications, we suggest referencing: Andre, A. N., Sandoz, P., Mauze, B., Jacquot, M., & Laurent, G. J. (2020). Sensing one nanometer over ten centimeters: A micro-encoded target for visual in-plane position measurement. IEEE/ASME Trans. Mechatronics.

## About

The Vernier Library is written and maintained by researchers with the FEMTO-ST Institute located in Besançon, France.

Authors: Antoine N. André, Guillaume J. Laurent, Patrick Sandoz

Contact: [vernier@femto-st.fr](mailto:vernier@femto-st.fr)

Copyright (c) 2018-2023 ENSMM, UFC, CNRS.

