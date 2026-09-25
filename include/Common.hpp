/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef COMMON_HPP
#define COMMON_HPP

// preventing the definitions of min and max as macros somewhere in <windows.h>
#define NOMINMAX  

#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <string>
#include <cmath>
#include <complex>
#include <ctime>
#include <vector>
#include <algorithm>

#include <Eigen/Dense>

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <rapidjson/document.hpp>

namespace vernier {

    // libstdc++ only declares the floating-point overloads of abs in namespace
    // std. Whether ::abs(double) also exists depends on the platform: on x86
    // Eigen pulls <xmmintrin.h> in, which reaches <stdlib.h> and its
    // "using std::abs", while on aarch64 <arm_neon.h> reaches nothing of the
    // sort. Unqualified abs(someDouble) therefore silently truncated its
    // argument through int abs(int) on Linux/aarch64 and broke the megarena
    // absolute decoding (issue #38). Library code should call std::abs; this
    // keeps the floating-point overloads reachable for the rest.
    using std::abs;

}

#include "Exception.hpp"
#include "Utils.hpp"

#include "Pose.hpp"
#include "PhasePlane.hpp"
#include "Rectangle.hpp"
#include "BufferedReader.hpp"

#endif