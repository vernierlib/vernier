/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Common.hpp"

namespace Eigen {
    typedef Array< unsigned char, Dynamic, Dynamic > ArrayXXu;
}

namespace vernier {

    const double PI = 3.14159265358979323846264338327950288419716939937510;
    const std::complex<double> I(0, 1);

    /** Converts a double, int or char to a std::string. */
    template <typename T> inline std::string to_string(T value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    void drawCameraFrame(cv::Mat & image);

    cv::Mat array2image(const Eigen::ArrayXXd & array);

    cv::Mat array2image(const Eigen::ArrayXXcd & array);

    Eigen::ArrayXXd image2array(const cv::Mat & image);

    void imageTo8UC1(const cv::Mat& image, cv::Mat& grayscaleImage);

    void arrayShow(const std::string windowTitle, const Eigen::ArrayXXd & array);

    void arrayShow(const std::string windowTitle, const Eigen::ArrayXXcd & array);

    /** Replace all the "Not A Number" terms of an array by zeros
     */
    void removeNanFromArray(Eigen::ArrayXXd & image);

    double angleInPiPi(double angle);

    const std::string currentDateTime();

}

#define PRINT(variable) { std::cout << "  "<< #variable << " at line " << __LINE__ << " = " << variable << std::endl; }

#endif