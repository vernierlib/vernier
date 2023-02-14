/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTEIGEN_HPP
#define TESTEIGEN_HPP

#include "Vernier.hpp"
#include "MatioFile.hpp"

namespace Vernier {

    class TestEigen {
    public:

        static void main() {
        }

        static void runAllTests() {
        }

        static double speed(unsigned long testCount) {
            Eigen::ArrayXXcd a = Eigen::ArrayXXcd::Random(1024, 768);
            Eigen::ArrayXXcd b = Eigen::ArrayXXcd::Random(1024, 768);

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                a *= b;
            }
            return toc(testCount);
        }

        static double speed2(unsigned long testCount) {
            Eigen::ArrayXXcd a = Eigen::ArrayXXcd::Random(1024, 768);
            Eigen::ArrayXXcd b = Eigen::ArrayXXcd::Random(1024, 768);

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                std::complex<double>* p = a.data();
                std::complex<double>* s = b.data();
                int n = 1024 * 768;
                for (; n--; p++, s++)
                    *p += (*s);
            }
            return toc(testCount);
        }
    };
}

#endif