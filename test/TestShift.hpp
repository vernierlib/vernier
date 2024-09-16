/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTSHIFTFOURIERTRANSFORM_HPP
#define TESTSHIFTFOURIERTRANSFORM_HPP

#include "Vernier.hpp"
#include "UnitTest.hpp"

namespace Vernier {

    class TestShift {
    public:

        static void main2() {
            std::cout << "============ Shift example 2 ========== " << std::endl;
            Eigen::ArrayXXcd in(5, 5);
            Eigen::ArrayXXcd out = Eigen::ArrayXXcd::Zero(5, 5);

            in << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25;

            Spectrum::shift(in, out);

            std::cout << "In = " << in << std::endl;
            std::cout << "Out = " << out << std::endl;
        }

        static void runAllTests() {
            Eigen::ArrayXXcd in3(5, 5);
            Eigen::ArrayXXcd result3(5, 5);
            Eigen::ArrayXXcd out3(5, 5);
            in3 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25;
            result3 << 19, 20, 16, 17, 18, 24, 25, 21, 22, 23, 4, 5, 1, 2, 3, 9, 10, 6, 7, 8, 14, 15, 11, 12, 13;
            Spectrum::shift(in3, out3);
            UNIT_TEST(areEqual(result3, out3));

            Eigen::ArrayXXcd in4(4, 4);
            Eigen::ArrayXXcd result4(4, 4);
            Eigen::ArrayXXcd out4(4, 4);
            in4 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;
            result4 << 11, 12, 9, 10, 15, 16, 13, 14, 3, 4, 1, 2, 7, 8, 5, 6;
            Spectrum::shift(in4, out4);
            UNIT_TEST(areEqual(result4, out4));
        }

        static double speed(unsigned long testCount) {
            Eigen::ArrayXXcd in = Eigen::ArrayXXcd::Random(1024, 768);
            Eigen::ArrayXXcd out(in);

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                Spectrum::shift(in, out);
            }
            return toc(testCount);
        }

        static double speed2(unsigned long testCount) {
            Eigen::ArrayXXcd in = Eigen::ArrayXXcd::Random(1024, 768);

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                Spatial::shift(in);
            }
            return toc(testCount);
        }
    };
}

#endif