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

#include "Exception.hpp"
#include "Utils.hpp"

#include "Pose.hpp"
#include "PhasePlane.hpp"
#include "Rectangle.hpp"
#include "BufferedReader.hpp"

namespace vernier {

    /** Compute backend used for the phase computations.
     *
     * CUDA is only usable when the library was built with -DUSE_CUDA=ON and a
     * CUDA device is present at runtime; ask cudaAvailable() before selecting it,
     * or be ready to catch the exception.
     */
    enum class Backend {
        CPU,
        CUDA
    };

    /** Returns true if the library was built with CUDA support and a device is present. */
    bool cudaAvailable();

    /** Throws if the given backend cannot be used on this build and machine.
     *
     * Kept in one place so every layer that accepts a backend rejects an
     * impossible one the same way, and says why: built without CUDA is a
     * different problem from built with it but no device fitted.
     */
    void requireBackendAvailable(Backend backend);
}

#endif