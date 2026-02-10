/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Spectrum.hpp"
#include "PatternPhase.hpp"
#include "eigen-matio/MatioFile.hpp"
#include "UnitTest.hpp"

using namespace vernier;

void mainPeakHalfPlane() {
    Eigen::MatioFile file("data/TestSpectrumPeak.mat", MAT_ACC_RDONLY);

    Eigen::ArrayXXcd table(1, 1);
    file.read_mat("spectrumPattern", table);

    Eigen::ArrayXXd mainPeakRef1(1, 1), mainPeakRef2(1, 1);
    file.read_mat("mainPeak1", mainPeakRef1);
    file.read_mat("mainPeak2", mainPeakRef2);
    std::cout << "main peak reference 1 : \n" << mainPeakRef1 << std::endl;
    std::cout << "main peak reference 2 : \n" << mainPeakRef2 << std::endl;

    Eigen::Vector3d mainPeak1, mainPeak2;
    mainPeakHalfPlane(table, mainPeak1, mainPeak2);

    std::cout << "main peak new method 1 : \n" << mainPeak1 << std::endl;
    std::cout << "main peak new method 2 : \n" << mainPeak2 << std::endl;

}

void mainShift() {
    std::cout << "============ Shift example 2 ========== " << std::endl;
    Eigen::ArrayXXcd in(5, 5);
    Eigen::ArrayXXcd out = Eigen::ArrayXXcd::Zero(5, 5);

    in << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25;

    shift(in, out);

    std::cout << "In = " << in << std::endl;
    std::cout << "Out = " << out << std::endl;
}

void mainAngularCut() {
    std::cout << "============ AngleCut example ========== " << std::endl;
    Eigen::ArrayXXcd in = Eigen::ArrayXXcd::Ones(20, 20);

    applyAngularCut(in, PI, PI/6);

    std::cout << "In = " << std::endl << in << std::endl;
}

void mainGaussianFilter() {
    Eigen::ArrayXXcd array = Eigen::ArrayXXcd::Ones(10, 10);
    applyGaussianFilter(array, 3, 4, 1);
    std::cout << array << std::endl;
}

void testShift() {
    START_UNIT_TEST;
    
    Eigen::ArrayXXcd in3(5, 5);
    Eigen::ArrayXXcd result3(5, 5);
    Eigen::ArrayXXcd out3(5, 5);
    in3 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25;
    result3 << 19, 20, 16, 17, 18, 24, 25, 21, 22, 23, 4, 5, 1, 2, 3, 9, 10, 6, 7, 8, 14, 15, 11, 12, 13;
    shift(in3, out3);
    UNIT_TEST(areEqual(result3, out3));

    Eigen::ArrayXXcd in4(4, 4);
    Eigen::ArrayXXcd result4(4, 4);
    Eigen::ArrayXXcd out4(4, 4);
    in4 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;
    result4 << 11, 12, 9, 10, 15, 16, 13, 14, 3, 4, 1, 2, 7, 8, 5, 6;
    shift(in4, out4);
    UNIT_TEST(areEqual(result4, out4));
}

void testPeakHalfPlane() {
    
    START_UNIT_TEST;
    Eigen::MatioFile file("data/TestSpectrumPeak.mat", MAT_ACC_RDONLY);

    Eigen::ArrayXXcd table(1, 1);
    file.read_mat("spectrumPattern", table);

    Eigen::ArrayXXd mainPeakRef1(1, 1), mainPeakRef2(1, 1);
    file.read_mat("mainPeak1", mainPeakRef1);
    file.read_mat("mainPeak2", mainPeakRef2);

    Eigen::Vector3d mainPeak1, mainPeak2;
    mainPeakHalfPlane(table, mainPeak1, mainPeak2);

    UNIT_TEST(areEqual(mainPeak1(0), mainPeakRef1(1) - 1));
    UNIT_TEST(areEqual(mainPeak1(1), mainPeakRef1(0) - 1));

    UNIT_TEST(areEqual(mainPeak2(0) - 1, mainPeakRef2(1)));
    UNIT_TEST(areEqual(mainPeak2(1), mainPeakRef2(0) - 1));
}

void testPeaksSearch() {
    
    START_UNIT_TEST;
    Eigen::MatioFile file("data/TestSpectrumPeak.mat", MAT_ACC_RDONLY);

    Eigen::ArrayXXcd table(1, 1);
    file.read_mat("spectrumPattern", table);

    Eigen::ArrayXXd mainPeakRef1(1, 1), mainPeakRef2(1, 1);
    file.read_mat("mainPeak1", mainPeakRef1);
    file.read_mat("mainPeak2", mainPeakRef2);

    Eigen::Vector3d mainPeak1, mainPeak2;
    Eigen::ArrayXXd magnitude = table.abs();
    PatternPhase p;
    p.peaksSearch(magnitude, mainPeak1, mainPeak2);

    UNIT_TEST(areEqual(mainPeak1(0), mainPeakRef1(1) - 1));
    UNIT_TEST(areEqual(mainPeak1(1), mainPeakRef1(0) - 1));

    UNIT_TEST(areEqual(mainPeak2(0) - 1, mainPeakRef2(1)));
    UNIT_TEST(areEqual(mainPeak2(1), mainPeakRef2(0) - 1));
}

double speedShift(unsigned long testCount) {
    Eigen::ArrayXXcd in = Eigen::ArrayXXcd::Random(1024, 768);
    Eigen::ArrayXXcd out(in);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        shift(in, out);
    }
    return toc(testCount);
}

double speedGaussianFilter(unsigned long testCount = 10) {
    Eigen::ArrayXXcd filter = Eigen::ArrayXXcd::Ones(1024, 1024);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        applyGaussianFilter(filter, 712, 812, 10.0);
    }
    return toc(testCount);
}

double speedPeakHalfPlane(unsigned long testCount) {
    Eigen::ArrayXXcd spatial = Eigen::ArrayXXcd::Random(1024, 1024);
    Eigen::Vector3d mainPeak1, mainPeak2;

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        mainPeakHalfPlane(spatial, mainPeak1, mainPeak2);
    }

    return toc(testCount);
}

double speedPeaksSearch(unsigned long testCount) {
    Eigen::ArrayXXd spatial = Eigen::ArrayXXd::Random(1024, 1024);
    Eigen::Vector3d mainPeak1, mainPeak2;
    PatternPhase p;
    
    tic();
    for (unsigned long i = 0; i < testCount; i++) {  
        p.peaksSearch(spatial, mainPeak1, mainPeak2);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {

    testShift();
    testPeakHalfPlane();
    testPeaksSearch();
    
    //PRINT(speedPeakHalfPlane(1000))
    //PRINT(speedPeaksSearch(1000))
    
    return EXIT_SUCCESS;
}