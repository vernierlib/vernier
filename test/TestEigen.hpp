/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTEIGEN_HPP
#define TESTEIGEN_HPP

#include "Vernier.hpp"
#include "UnitTest.hpp"

#include "MatioFile.hpp"

namespace vernier {

    class TestEigen {
    public:

        static void main() {
            Eigen::ArrayXXcd spatial(8, 4);
            
            spatial << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32;

            std::cout << "initial = " << spatial << std::endl;
            
//            spatial.transposeInPlace();
//            spatial.colwise().reverseInPlace();
            spatial.reverseInPlace();
            std::cout << "reverse = " << spatial << std::endl;
            
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