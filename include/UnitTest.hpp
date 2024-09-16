/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef UNITTEST_HPP
#define UNITTEST_HPP

// preventing the definitions of min and max as macros somewhere in <windows.h>
#define NOMINMAX  

#include <iostream>
#include <string>
#include <complex>
#include <random>
#include <Eigen/Dense>
#include "Pose.hpp"

const double DEFAULT_DOUBLE_EQUALITY_TOLERANCE = 0.00000000000001;
const double DEFAULT_FLOAT_EQUALITY_TOLERANCE = 0.0000001;

/** Returns the amount of milliseconds elapsed. Works on windows, macos and linux. */
unsigned long getTick();

void tic();

double toc(unsigned long testCount = 1);

bool areEqual(double x, double y, double tolerance = DEFAULT_DOUBLE_EQUALITY_TOLERANCE);

bool areSignificandEqual(double x, double y, double tolerance = DEFAULT_DOUBLE_EQUALITY_TOLERANCE);

bool areEqual(std::complex<double> x, std::complex<double> y, double tolerance = DEFAULT_DOUBLE_EQUALITY_TOLERANCE);

bool areEqual(int x, int y);

bool areEqual(Vernier::Pose a, Vernier::Pose b, double tolerance = DEFAULT_DOUBLE_EQUALITY_TOLERANCE);

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

double randomDouble(double min,double max);

double randomDouble(double max = 1.0);

/** Remove the path before the filename */
std::string removePath(std::string filename);

#define UNIT_TEST(condition) { if (!(condition)) { std::cout<<"[Vernier unit test at "<<removePath(__FILE__)<<":"<<__LINE__<<"] Failed!!!!!!!"<<std::endl; exit(EXIT_FAILURE); } else std::cout<<"[Vernier unit test at "<<removePath(__FILE__)<<":"<<__LINE__ <<"] Passed"<<std::endl; }

#define START_UNIT_TEST { std::cout<<"[Vernier unit test at "<<removePath(__FILE__)<<":"<<__LINE__<<"] Starting..."<<std::endl; }

#define REPEAT_TEST(test,count) { for(int i=0;i<count;i++) {test;} }

#define TEST_EQUALITY(a,b,tol) UNIT_TEST(areEqual(a,b,tol))

#endif