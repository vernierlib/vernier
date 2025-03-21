/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Utils.hpp"
#include "UnitTest.hpp"

using namespace vernier;
using namespace std;

void testAreEquals1() {
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

void testAreEquals2() {
    float a = 0.300000001e4;
    float b = 0.300000010e4;
    float c = 0.300000100e4;
    UNIT_TEST(areSignificandEqual(a, b));
    UNIT_TEST(areSignificandEqual(a, c) == false);
}

void testAreEquals3() {
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

void testAreEquals4() {
    int a = 3;
    int b = 3;
    int c = 4;
    UNIT_TEST(areEqual(a, b));
    UNIT_TEST(areEqual(a, c) == false);
}

int main(int argc, char** argv) {

    START_UNIT_TEST;
    
    testAreEquals1();
    testAreEquals2();
    testAreEquals3();
    testAreEquals4();

    return EXIT_SUCCESS;
}