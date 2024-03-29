/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Common.hpp"

namespace Eigen {
    typedef Array< unsigned char, Dynamic, Dynamic > ArrayXXu;
}

namespace Vernier {

    const double PI = 3.14159265358979323846264338327950288419716939937510;
    const std::complex<double> I(0, 1);
    const int QRCODE_33P = 33;
    const int QRCODE_37P = 37;
    const int QRCODE_39P = 39;

    /** Converts a double, int or char to a std::string. */
    template <typename T> inline std::string toString(T value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }

#ifdef USE_OPENCV
    void drawCameraFrame(cv::Mat & image);

    cv::Mat array2image(Eigen::ArrayXXd & array);

    cv::Mat array2image(Eigen::ArrayXXcd & array);
    
    Eigen::ArrayXXd image2array(cv::Mat & image);
#endif // USE_OPENCV

    void arrayShow(const std::string windowTitle, Eigen::ArrayXXd & array);

    Eigen::ArrayXXd readPGMData(std::string filename, int & numrows, int & numcols);

    Eigen::ArrayXXd readPGM(std::string filename);

    /** Remove all the "Not A Number" terms of an array
     */
    void removeNanFromArray(Eigen::ArrayXXd & image);
}

#endif