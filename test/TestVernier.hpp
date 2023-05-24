/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTVERNIER_HPP
#define TESTVERNIER_HPP

#include "Vernier.hpp"

#include <fstream>
#include <ctime>
#include <time.h>

#include "TestAbsoluteDecoding.hpp"
#include "TestCell.hpp"
#include "TestFourierTransform.hpp"
#include "TestGaussianFilter.hpp"
#include "TestRegressionPlane.hpp"
#include "TestPatternPhase.hpp"
#include "TestSpatial.hpp"
#include "TestSpectrum.hpp"
#include "TestThumbnail.hpp"

#include "TestPatternDetector.hpp"
#include "TestQRCodeDetector.hpp"
#include "TestQRMarkerDetector.hpp"

#include "TestPattern.hpp"

#include "TestBufferedReader.hpp"
#include "TestUtils.hpp"

#include <iomanip>
#include <cmath>
#include <limits>
#include <random>

namespace Vernier {

    class TestVernier {
    public:

        static void runAllTests() {
            std::cout << "========= Unit tests =========\n" << std::endl;

            std::cout << "unit test: absolute decoding";
            TestAbsoluteDecoding::runAllTests();
            std::cout << "			ok" << std::endl;

            std::cout << "unit test: cell";
            TestCell::runAllTests();
            std::cout << "			ok" << std::endl;

            std::cout << "unit test: fourier transform";
            TestFourierTransform::runAllTests();
            std::cout << "			ok" << std::endl;

            std::cout << "unit test: hyperGaussian filter";
            TestGaussianFilter::runAllTests();
            std::cout << "			ok" << std::endl;

            std::cout << "unit test: phase plane";
            TestRegressionPlane::runAllTests();
            std::cout << "			ok" << std::endl;

            std::cout << "unit test: phase retrieving";
            TestPatternPhase::runAllTests();
            std::cout << "			ok" << std::endl;

            std::cout << "unit test: spatial";
            TestSpatial::runAllTests();
            std::cout << "			ok" << std::endl;

            std::cout << "unit test: spectrum";
            TestSpectrum::runAllTests();
            std::cout << "			ok" << std::endl;
        }

        static void speedTests(unsigned long testCount) {
            std::cout << "========= Speed tests =========\n" << std::endl;

            std::cout << "fourier transform : " << TestFourierTransform::speed(testCount) << " ms" << std::endl;
            std::cout << "Fourier transform shift : " << TestSpectrum::speedShift(testCount) << " ms" << std::endl;
            std::cout << "main peak search : " << TestSpectrum::speedPeaks(testCount) << " ms" << std::endl;
            std::cout << "hyper gaussian filter : " << TestGaussianFilter::speed2(testCount) << " ms" << std::endl;
            std::cout << "unwrapping : " << TestSpatial::speed(testCount) << " ms" << std::endl;
            std::cout << "phase plane : " << TestRegressionPlane::speed(testCount) << " ms" << std::endl;
            std::cout << "phase mapping : " << TestPatternPhase::speed(testCount) << " ms" << std::endl;
            std::cout << "find code position : " << TestAbsoluteDecoding::speedFindCode(testCount) << " ms" << std::endl;
            std::cout << "thumbnail compute : " << TestThumbnail::speed(testCount) << " ms" << std::endl;

            std::cout << "\n" << std::endl;

            //std::cout << "periodic pattern computing time : " << Vernier::TestPatternDetector::speedPeriodic(testCount) << std::endl;
            //std::cout << "megarena computing time : " << Vernier::TestPatternDetector::speedMegarena(testCount) << std::endl;

            std::cout << "\n" << std::endl;
        }

        static void toSvnLogs() {
            std::ofstream file("speedLogs.txt", std::ios::app);

            std::cout << "====== Speed tests log ======" << std::endl;
            file << "====== Speed tests log ======" << std::endl;

            time_t result = time(NULL);

            std::cout << "computing speed [FOURIER TRANSFORM]" << std::endl;
            file << "fourier transform : " << TestFourierTransform::speed(1000) << " ms" << std::endl;

            std::cout << "computing speed [FOURIER TRANSFROM SHIFT]" << std::endl;
            file << "Fourier transform shift : " << TestSpectrum::speedShift(1000) << " ms" << std::endl;

            std::cout << "computing speed [MAIN PEAK SEARCH]" << std::endl;
            file << "search main peak circle : " << TestSpectrum::speedPeaks(1000) << " ms" << std::endl;

            std::cout << "computing speed [HYPER GAUSSIAN FILTER]" << std::endl;
            file << "hyper gaussian filter : " << TestGaussianFilter::speed2(1000) << " ms" << std::endl;

            std::cout << "computing speed [UNWRAPPING PHASE]" << std::endl;
            file << "unwrapping : " << TestSpatial::speed(1000) << " ms" << std::endl;

            std::cout << "computing speed [LEAST SQUARES PLANE]" << std::endl;
            file << "phase plane : " << TestRegressionPlane::speed(1000) << " ms" << std::endl;

            std::cout << "computing speed [PHASE MAPPING]" << std::endl;
            file << "compute phase mapping : " << TestPatternPhase::speed(1000) << " ms" << std::endl;

            std::cout << "computing speed [THUMBNAIL COMPUTE]" << std::endl;
            file << "thumbnail compute : " << TestThumbnail::speed(1000) << " ms" << std::endl;

            std::cout << "computing speed [FIND CODE POSITION]" << std::endl;
            file << "find code position : " << TestAbsoluteDecoding::speedFindCode(1000) << " ms\n" << std::endl;

            //std::cout << "computing speed [PERIODIC PATTERN REFERENCING]" << std::endl;
            //file << "periodic pattern referencing : " << TestPatternDetector::speedPeriodic(1000) << " ms" << std::endl;

            //std::cout << "computing speed [MEGARENA PATTERN REFERENCING]" << std::endl;
            //file << "megarena pattern referencing : " << TestPatternDetector::speedMegarena(1000) << " ms" << std::endl;
        }
    };
}

#endif /* TESTVERNIER_HPP */