/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTGAUSSIANFILTER_HPP
#define TESTGAUSSIANFILTER_HPP

#include "Vernier.hpp"
#include "UnitTest.hpp"
#include "eigen-matio/MatioFile.hpp"

namespace vernier {

    class TestGaussianFilter {
    public:

        static void main() {       
            GaussianFilter filter(1);
            Eigen::ArrayXXcd array = Eigen::ArrayXXcd::Ones(10, 10);
            filter.applyTo(array, 3, 4);
            std::cout << array << std::endl;
            std::cout << "kernel size = " << filter.getKernel().rows() << std::endl;
        }

        static void main2() {
            Eigen::ArrayXXcd array = Eigen::ArrayXXcd::Ones(10, 10);
            gaussianFilter(array, 3, 4, 1);
            std::cout << array << std::endl;
        }

        static void runAllTests() {
            Eigen::ArrayXXcd filter1 = Eigen::ArrayXXcd::Ones(512, 768);
            gaussianFilter(filter1, 256, 128, 15);
            
            Eigen::ArrayXXcd filter2 = Eigen::ArrayXXcd::Ones(512, 768);
            GaussianFilter(15).applyTo(filter2, 256, 128);
            UNIT_TEST(areEqual(filter2, filter1));
        }

        static double speed(unsigned long testCount = 10) {
            Eigen::ArrayXXcd array = Eigen::ArrayXXcd::Ones(1024, 1024);
            GaussianFilter filter(1.0);

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                filter.applyTo(array, 512, 512);
            }
            return toc(testCount);
        }

        static double speed2(unsigned long testCount = 10) {
            Eigen::ArrayXXcd filter = Eigen::ArrayXXcd::Ones(1024, 1024);

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                gaussianFilter(filter, 512, 512, 1.0);
            }
            return toc(testCount);
        }
    };
}

#endif