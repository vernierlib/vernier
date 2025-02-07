/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "GaussianFilter.hpp"
#include "UnitTest.hpp"

using namespace vernier;
using namespace std;

void main1() {
    GaussianFilter filter(1);
    Eigen::ArrayXXcd array = Eigen::ArrayXXcd::Ones(10, 10);
    filter.applyTo(array, 3, 4);
    std::cout << array << std::endl;
    std::cout << "kernel size = " << filter.getKernel().rows() << std::endl;
}

void main2() {
    Eigen::ArrayXXcd array = Eigen::ArrayXXcd::Ones(10, 10);
    gaussianFilter(array, 3, 4, 1);
    std::cout << array << std::endl;
}

void runAllTests() {
    
    START_UNIT_TEST;
    
    Eigen::ArrayXXcd filter1 = Eigen::ArrayXXcd::Ones(512, 768);
    gaussianFilter(filter1, 256, 128, 15);

    Eigen::ArrayXXcd filter2 = Eigen::ArrayXXcd::Ones(512, 768);
    GaussianFilter(15).applyTo(filter2, 256, 128);
    UNIT_TEST(areEqual(filter2, filter1));
}

double speed(unsigned long testCount = 10) {
    Eigen::ArrayXXcd array = Eigen::ArrayXXcd::Ones(1024, 1024);
    GaussianFilter filter(1.0);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        filter.applyTo(array, 512, 512);
    }
    return toc(testCount);
}

double speed2(unsigned long testCount = 10) {
    Eigen::ArrayXXcd filter = Eigen::ArrayXXcd::Ones(1024, 1024);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        gaussianFilter(filter, 512, 512, 1.0);
    }
    return toc(testCount);
}

int main(int argc, char** argv) {

    runAllTests();

    return EXIT_SUCCESS;
}