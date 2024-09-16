/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTUTILS_HPP
#define TESTUTILS_HPP

#include "Vernier.hpp"
#include "UnitTest.hpp"

namespace Vernier {

    class TestUtils {
    public:

        static void main() {
        }
        
        static double speed(unsigned long testCount) {
            return -1.0;
        }
        
        static void runAllTests() {
            testAreEquals1();
            testAreEquals2();
            testAreEquals3();
            testAreEquals4();
        }

        static void testAreEquals1() {
            double a = 0.3000000000000001e4;
            double b = 0.3000000000000010e4;
            double c = 0.3000000000000100e4;
            UNIT_TEST(areSignificandEqual(a, b));
            UNIT_TEST(areSignificandEqual(a, c) == false);
            UNIT_TEST(areSignificandEqual(0.0, -5.55112e-17));
            a = 2.0;
            b = 1.99999;
            UNIT_TEST(areSignificandEqual(a, b, 0.01));
            UNIT_TEST(areEqual(0.0, -5.55112e-17));
            UNIT_TEST(areEqual(10.3, 10.30000000000001));
        }

        static void testAreEquals2() {
            float a = 0.300000001e4;
            float b = 0.300000010e4;
            float c = 0.300000100e4;
            UNIT_TEST(areSignificandEqual(a, b));
            UNIT_TEST(areSignificandEqual(a, c) == false);
        }

        static void testAreEquals3() {
            Eigen::ArrayXXd a(3, 2);
            Eigen::ArrayXXd b(3, 2);
            Eigen::ArrayXXd c(2, 3);
            a << 1, 1, 2, 3, 5, 3;
            b << 1, 1, 2, 3, 6, 3;
            c << 1, 1, 2, 3, 5, 3;
            UNIT_TEST(areEqual(a, a));
            UNIT_TEST(areEqual(a, b) == false);
            UNIT_TEST(areEqual(a, c) == false);
        }

        static void testAreEquals4() {
            int a = 3;
            int b = 3;
            int c = 4;
            UNIT_TEST(areEqual(a, b));
            UNIT_TEST(areEqual(a, c) == false);
        }
       
    };
}

#endif