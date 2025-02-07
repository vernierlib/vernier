/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#include "UnitTest.hpp"

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
    return (fabs(x - y) < 2 * tolerance);
}

bool areSignificandEqual(double x, double y, double tolerance) {
    if (fabs(x) > 1.0) {
        double mx;
        int ex;
        mx = frexp(x, &ex);
        return (fabs(x / (1 << ex) - y / (1 << ex)) < 2 * tolerance);
    } else
        return (fabs(x - y) < 2 * tolerance);
}

bool areEqual(std::complex<double> x, std::complex<double> y, double tolerance) {
    return areEqual(x.real(), y.real(), tolerance) && areEqual(x.imag(), y.imag(), tolerance);
}

bool areEqual(int x, int y) {
    return x == y;
}

bool areEqual(vernier::Pose a, vernier::Pose b, double tolerance) {
    return areEqual(a.x, b.x, tolerance) 
            && areEqual(a.y, b.y, tolerance) 
            && areEqual(a.z, b.z, tolerance) 
            && areEqual(a.alpha, b.alpha, tolerance) 
            && areEqual(a.beta, b.beta, tolerance) 
            && areEqual(a.gamma, b.gamma, tolerance) 
            && areEqual(a.pixelSize, b.pixelSize, tolerance); 
}

std::mt19937 generator(25); // for tests reproductibility

double randomDouble(double min, double max) {
    return std::uniform_real_distribution<double>(min, max)(generator);
}

double randomDouble(double max) {
    return randomDouble(0.0, max);
}

std::string removePath(std::string filename) {
    int i = filename.length();
    while (i >= 0 && filename[i] != '/' && filename[i] != '\\') i--;
    if (i >= 0)
        return filename.substr(i + 1, filename.length() - i - 1);
    else
        return filename;
}
