/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Spectrum.hpp"
#include "eigen-matio/MatioFile.hpp"
#include "UnitTest.hpp"

using namespace vernier;

void main1() {
    Eigen::MatioFile file("data/TestSpectrumPeak.mat", MAT_ACC_RDONLY);

    Eigen::ArrayXXcd table(1, 1);
    file.read_mat("spectrumPattern", table);

    Eigen::ArrayXXd mainPeakRef1(1, 1), mainPeakRef2(1, 1);
    file.read_mat("mainPeak1", mainPeakRef1);
    file.read_mat("mainPeak2", mainPeakRef2);
    std::cout << "main peak reference 1 : \n" << mainPeakRef1 << std::endl;
    std::cout << "main peak reference 2 : \n" << mainPeakRef2 << std::endl;

    double period = 20;
    Eigen::Vector3d mainPeak1, mainPeak2;
    Spectrum::mainPeakCircle(table, mainPeak1, mainPeak2, period);

    std::cout << "main peak new method 1 : \n" << mainPeak1 << std::endl;
    std::cout << "main peak new method 2 : \n" << mainPeak2 << std::endl;

}

void main2() {
    std::cout << "============ Shift example 2 ========== " << std::endl;
    Eigen::ArrayXXcd in(5, 5);
    Eigen::ArrayXXcd out = Eigen::ArrayXXcd::Zero(5, 5);

    in << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25;

    Spectrum::shift(in, out);

    std::cout << "In = " << in << std::endl;
    std::cout << "Out = " << out << std::endl;
}

void test1() {
    START_UNIT_TEST;
    
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

void test2() {
    
    START_UNIT_TEST;
    Eigen::MatioFile file("data/TestSpectrumPeak.mat", MAT_ACC_RDONLY);

    Eigen::ArrayXXcd table(1, 1);
    file.read_mat("spectrumPattern", table);

    Eigen::ArrayXXd mainPeakRef1(1, 1), mainPeakRef2(1, 1);
    file.read_mat("mainPeak1", mainPeakRef1);
    file.read_mat("mainPeak2", mainPeakRef2);

    double period = 20;
    Eigen::Vector3d mainPeak1, mainPeak2;
    Spectrum::mainPeakCircle(table, mainPeak1, mainPeak2, period);

    UNIT_TEST(areEqual(mainPeak1(0), mainPeakRef1(1) - 1));
    UNIT_TEST(areEqual(mainPeak1(1), mainPeakRef1(0) - 1));

    UNIT_TEST(areEqual(mainPeak2(0) - 1, mainPeakRef2(1)));
    UNIT_TEST(areEqual(mainPeak2(1), mainPeakRef2(0) - 1));
}

double speedShift(unsigned long testCount) {
    Eigen::ArrayXXcd spatial = Eigen::ArrayXXcd::Random(1024, 1024);
    Eigen::ArrayXXcd spectral(spatial);
    Eigen::ArrayXXcd out(1024, 1024);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        Spectrum::shift(spatial, out);
    }

    return toc(testCount);
}

double speed(unsigned long testCount) {
    Eigen::ArrayXXcd in = Eigen::ArrayXXcd::Random(1024, 768);
    Eigen::ArrayXXcd out(in);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        Spectrum::shift(in, out);
    }
    return toc(testCount);
}


double speedPeaks(unsigned long testCount) {
    Eigen::ArrayXXcd spatial = Eigen::ArrayXXcd::Random(1024, 1024);

    Eigen::Vector3d mainPeak1, mainPeak2;

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        Spectrum::mainPeakCircle(spatial, mainPeak1, mainPeak2, 14);
    }

    return toc(testCount);
}

double speedPeaksParallelogram(unsigned long testCount) {
    Eigen::ArrayXXcd spatial = Eigen::ArrayXXcd::Random(1024, 1024);
    spatial.block(0, 0, 50, spatial.cols()) = 0;
    spatial.block(0, 0, spatial.rows(), 50) = 0;
    spatial.block(0, spatial.cols() - 50, spatial.rows(), 50) = 0;
    spatial.block(spatial.rows() - 50, 0, 50, spatial.cols() - 50) = 0;

    Eigen::Vector3d mainPeak1, mainPeak2;

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        Spectrum::mainPeakPerimeter(spatial, mainPeak1, mainPeak2);
    }

    return toc(testCount);
}

double speedPeaksEnergy(unsigned long testCount) {
    Eigen::ArrayXXcd spatial = Eigen::ArrayXXcd::Random(1024, 1024);
    spatial.block(0, 0, 50, spatial.cols()) = 0;
    spatial.block(0, 0, spatial.rows(), 50) = 0;
    spatial.block(0, spatial.cols() - 50, spatial.rows(), 50) = 0;
    spatial.block(spatial.rows() - 50, 0, 50, spatial.cols() - 50) = 0;

    Eigen::Vector3d mainPeak1, mainPeak2;

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        Spectrum::mainPeak4Search(spatial, mainPeak1, mainPeak2);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {

    test1();
    test2();

    return EXIT_SUCCESS;
}