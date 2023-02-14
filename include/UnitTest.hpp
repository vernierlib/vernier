/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef UNITTEST_HPP
#define UNITTEST_HPP

#include "Exception.hpp"

namespace Vernier {

    const double DEFAULT_DOUBLE_EQUALITY_TOLERANCE = 0.00000000000001;
    const double DEFAULT_FLOAT_EQUALITY_TOLERANCE = 0.0000001;
    
    /** Returns the amount of milliseconds elapsed. Works on windows, macos and linux. */
    unsigned long getTick();

    void tic();

    double toc(unsigned long testCount = 1);

    bool areEqual(double x, double y, double tolerance = DEFAULT_DOUBLE_EQUALITY_TOLERANCE);

    bool areEqual(float x, float y, float tolerance = DEFAULT_FLOAT_EQUALITY_TOLERANCE);

    bool areEqual(std::complex<double> x, std::complex<double> y, double tolerance = DEFAULT_DOUBLE_EQUALITY_TOLERANCE);

    bool areEqual(std::complex<float> x, std::complex<float> y, float tolerance = DEFAULT_FLOAT_EQUALITY_TOLERANCE);

    bool areEqual(int x, int y);

    template<typename _Scalar, int _Rows, int _Cols>
    bool areEqual(Eigen::Array<_Scalar, _Rows, _Cols>& x, Eigen::Array<_Scalar, _Rows, _Cols>& y, double tolerance = DEFAULT_DOUBLE_EQUALITY_TOLERANCE) {
        if (y.rows() != y.rows() || x.cols() != y.cols()) {
            return false;
        } else {
            bool result = true;
            int row = 0;
            while (row < x.rows() && result) {
                int col = 0;
                while (col < x.cols() && result) {
                    result = areEqual(x(row, col), y(row, col), tolerance);
                    col++;
                }
                row++;
            }
            return result;
        }
    };

    template<typename _Scalar, int _Rows, int _Cols>
    bool areEqual(Eigen::Matrix<_Scalar, _Rows, _Cols>& x, Eigen::Matrix<_Scalar, _Rows, _Cols>& y, double tolerance = DEFAULT_DOUBLE_EQUALITY_TOLERANCE) {
        if (y.rows() != y.rows() || x.cols() != y.cols()) {
            return false;
        } else {
            bool result = true;
            int row = 0;
            while (row < x.rows() && result) {
                int col = 0;
                while (col < x.cols() && result) {
                    result = areEqual(x(row, col), y(row, col), tolerance);
                    col++;
                }
                row++;
            }
            return result;
        }
    };

}

#define UNIT_TEST(condition) if (!(condition)) throw Exception(std::string("Unit test failed: ")+std::string(#condition),__FILE__,__LINE__); else std::cout<<std::string("Unit test passed: ")<<std::string(#condition)<<std::endl;

#endif