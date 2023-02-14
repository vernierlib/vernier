/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "UnitTest.hpp"

namespace Vernier {
    
#ifdef _MSC_VER
#include <windows.h>

    unsigned long getTick() {
        return (unsigned long) (GetTickCount());
    }
#else

    unsigned long getTick() {
        return (unsigned long) (clock() / 1000);
    }
#endif

    static unsigned long startTime;

    void tic() {
        startTime = getTick();
    }

    double toc(unsigned long testCount) {
        return (double) (getTick() - startTime) / testCount;
    }

    bool areEqual(double x, double y, double tolerance) {
        if (fabs(x) > 1.0) {
            double mx, my;
            int ex, ey;
            mx = frexp(x, &ex);
            my = frexp(y, &ey);
            return ((ex == ey) && (mx < my + tolerance) && (mx > my - tolerance));
        } else
            return (fabs(x - y) < 2 * tolerance);
    }

    bool areEqual(float x, float y, float tolerance) {
        if (fabs(x) > 1.0) {
            float mx, my;
            int ex, ey;
            mx = frexp(x, &ex);
            my = frexp(y, &ey);
            return ((ex == ey) && (mx < my + tolerance) && (mx > my - tolerance));
        } else
            return (fabs(x - y) < 2 * tolerance);
    }

    bool areEqual(std::complex<double> x, std::complex<double> y, double tolerance) {
        return areEqual(x.real(), y.real(), tolerance) && areEqual(x.imag(), y.imag(), tolerance);
    }

    bool areEqual(std::complex<float> x, std::complex<float> y, float tolerance) {
        return areEqual(x.real(), y.real(), tolerance) && areEqual(x.imag(), y.imag(), tolerance);
    }

    bool areEqual(int x, int y) {
        return x == y;
    }

}