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

#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#endif //USE_OPENCV

#include <rapidjson/document.hpp>
#include <gdstk/gdstk.hpp>

#include "Exception.hpp"
#include "Utils.hpp"

#include "Pose.hpp"
#include "Plane.hpp"
#include "Rectangle.hpp"
#include "BufferedReader.hpp"

#endif