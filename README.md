# Vernier Library 

[![Information](https://img.shields.io/badge/info-on_website-purple.svg)](https://projects.femto-st.fr/vernier/en)
[![Documentation](https://img.shields.io/badge/doc-on_website-blue.svg)](https://vernierlib.github.io/)
[![Contact](https://img.shields.io/badge/contact-form-green.svg)](https://projects.femto-st.fr/vernier/en/contact) 
[![GithHb](https://img.shields.io/badge/sources-on_github-orange.svg)](https://github.com/vernierlib) 

The Vernier Library is an open-source C++ library for pose measurement of calibrated patterns with subpixel resolutions.

The library defines a collection of classes for detection and rendering different kind of calibrated patterns (periodic patterns, megarena patterns, QR-code-like and stamps patterns).

## Documentation

The [documentation](https://vernierlib.github.io/) is automatically generated from the code using Doxygen.

## Compatibility

The Vernier library is cross-platform and requires a C++14 compiler. Some platform/compiler combinations which have been tested are shown as follows.

* Visual C++ 2019/2022 on Windows (64-bit)
<!--- * GNU C++ 3.8.x on Cygwin -->
* Clang 19.1.7 on macOS 15 Sequoia

Users can build and run the unit tests on their platform/compiler.

## Installation

The library uses [CMake](https://cmake.org/) as a general build tool and 
[Doxygen](http://www.doxygen.org) to generate the documentation

Many packages are included inside the repositories ([see all dependencies](3rdparty/README.md)).

### Windows instructions

The package comes with all the 3rd parties for Windows 64bits except the OpenCV library.

Thus, you have to install OpenCV first:

1. Download the OpenCV release from [https://opencv.org/releases/](https://github.com/opencv/opencv/releases/download/4.7.0/opencv-4.7.0-windows.exe)
2. Unpack the self-extracting archive in a local directory, for example `C:\lib\opencv`
3. Set the environment variables as follows:
	- Go to `Control Panel -> System and Security -> System Advanced System Settings -> Environment Variables`
	- Add a new user variable with name `OpenCV_DIR` and value `C:\lib\opencv\build`
<!---     - Edit the user variable Path and add a new directory `C:\lib\opencv\build\x64\vc16\bin` (location of the dll files) -->

To get everything running with Visual Studio (and other EDI):

1. Open the directory with CMake
2. Generate the project for your preferred EDI
3. Restart the computer for system path updating
4. Open the generated project

OpenCV requires the Media Feature Pack for Windows to run, you may have to install it if some MF???.dll are missing.

### Linux instructions

With Linux, some dependencies must be installed first using the package manager:

	> sudo apt install cmake libtool libmatio

Then, you have to install OpenCV:

	> sudo apt install libopencv-dev

Optionnally you may install FFTW (recommanded for speed):

	> sudo apt install fftw

Finally, open a terminal and go to the directory of the package

	> mkdir build
	> cd build
	> cmake ..
	> make

### macOS instructions

With macOS, some dependencies must be installed first using homebrew:

	% brew install autoconfig automake cmake eigen libtool libmatio

Then, you have to install OpenCV:

	% brew install opencv

Optionnally you may install FFTW (recommanded for speed):

	% brew install fftw

Finally, open a terminal and go to the directory of the package

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

Copyright (c) 2018-2025 ENSMM, CNRS, UMLP.

