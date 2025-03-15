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

    template<typename _Tp, int _rows, int _cols, int _options, int _maxRows, int _maxCols>  inline
    void cv2eigen(const cv::Mat& src, Eigen::Array<_Tp, _rows, _cols, _options, _maxRows, _maxCols>& dst) {
        dst.resize(src.rows, src.cols);
        //CV_DbgAssert(src.rows == _rows && src.cols == _cols);
        if (!(dst.Flags & Eigen::RowMajorBit)) {
            const cv::Mat _dst(src.cols, src.rows, cv::traits::Type<_Tp>::value,
                    dst.data(), (size_t) (dst.outerStride() * sizeof (_Tp)));
            if (src.type() == _dst.type())
                transpose(src, _dst);
            else if (src.cols == src.rows) {
                src.convertTo(_dst, _dst.type());
                transpose(_dst, _dst);
            } else
                cv::Mat(src.t()).convertTo(_dst, _dst.type());
        } else {
            const cv::Mat _dst(src.rows, src.cols, cv::traits::Type<_Tp>::value,
                    dst.data(), (size_t) (dst.outerStride() * sizeof (_Tp)));
            src.convertTo(_dst, _dst.type());
        }
    }

    template<typename _Tp, int _rows, int _cols, int _options, int _maxRows, int _maxCols>  inline
    void eigen2cv(const Eigen::Array<_Tp, _rows, _cols, _options, _maxRows, _maxCols>& src, cv::Mat & dst) { 
        if (!(src.Flags & Eigen::RowMajorBit)) {
            cv::Mat _src(src.cols(), src.rows(), cv::traits::Type<_Tp>::value,
                    (void*) src.data(), src.outerStride() * sizeof (_Tp));
            transpose(_src, dst);
        } else {
            cv::Mat _src(src.rows(), src.cols(), cv::traits::Type<_Tp>::value,
                    (void*) src.data(), src.outerStride() * sizeof (_Tp));
            _src.copyTo(dst);
        }
    }

}

#endif