/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTSPECTRUM_HPP
#define TESTSPECTRUM_HPP

#include "Vernier.hpp"
#include "UnitTest.hpp"

#include "Spectrum.hpp"
#include "eigen-matio/MatioFile.hpp"

namespace Vernier {

    class TestSpectrum {
    public:

        static void main2() {
            Eigen::MatioFile file("testfiles/TestSpectrumPeak.mat", MAT_ACC_RDONLY);

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

        static void runAllTests() {
            Eigen::MatioFile file("testfiles/TestSpectrumPeak.mat", MAT_ACC_RDONLY);

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

        static double speedShift(unsigned long testCount) {
            Eigen::ArrayXXcd spatial = Eigen::ArrayXXcd::Random(1024, 1024);
            Eigen::ArrayXXcd spectral(spatial);
            Eigen::ArrayXXcd out(1024, 1024);

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                Spectrum::shift(spatial, out);
            }

            return toc(testCount);
        }

        static double speedPeaks(unsigned long testCount) {
            Eigen::ArrayXXcd spatial = Eigen::ArrayXXcd::Random(1024, 1024);

            Eigen::Vector3d mainPeak1, mainPeak2;

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                Spectrum::mainPeakCircle(spatial, mainPeak1, mainPeak2, 14);
            }

            return toc(testCount);
        }

        static double speedPeaksParallelogram(unsigned long testCount) {
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

        static double speedPeaksEnergy(unsigned long testCount) {
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
    };
}

#endif // !TESTSPECTRUMANALYSIS_HPP