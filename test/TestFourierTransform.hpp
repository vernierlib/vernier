/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTFOURIERTRANSFORM_HPP
#define TESTFOURIERTRANSFORM_HPP

#include "Vernier.hpp"
#include "UnitTest.hpp"
#include <complex>
#include <random>

namespace vernier {

    class TestFourierTransform {
    public:

        static void main() {
            Eigen::ArrayXXcd spatial(8, 4);
            Eigen::ArrayXXcd spectral(8, 4);

            spatial << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32;

            FourierTransform ft;
            std::cout << "spatial = " << spatial << std::endl;

            ft.compute(spatial, spectral);

            Eigen::ArrayXXcd spectralShifted(spectral.rows(), spectral.cols());
            Spectrum::shift(spectral, spectralShifted);

            std::cout << "spectral shifted = " << spectralShifted << std::endl;
        }

#ifdef USE_OPENCV

        static void main2() {

            // Creating the layout
            PatternLayout* layout = new PeriodicPatternLayout(15.0, 81, 61);

            // Setting the pose of the pattern in the camera frame for rendering
            double x = 6.0;
            double y = 3.0;
            double alpha = 0.1;
            double beta = 1.0;
            double gamma = 0.0;
            double pixelSize = 2.0;
            Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);

            Eigen::ArrayXXd array(1024, 512);
            Eigen::ArrayXXcd spatial;
            Eigen::ArrayXXcd spectrum;
            Eigen::ArrayXXcd spectrumShifted;
            Eigen::ArrayXXd spectrumAbs;
            FourierTransform ft;


            std::cout << "------------------------------------------------------------------" << std::endl;

            for (gamma = 0.0; gamma < 0.3; gamma += 0.01) {
                patternPose.gamma = gamma;
                std::cout << "Pattern pose: " << patternPose.toString() << std::endl;

                // Rendering
                layout->renderOrthographicProjection(patternPose, array);

                // Showing image
                cv::imshow("Spatial", array2image(array));

                // Fourier transform
                spatial.resize(array.rows(), array.cols());
                spatial.real() = array;
                ft.compute(spatial, spectrum);
                Spectrum::shift(spectrum, spectrumShifted);

                // Showing spectrum
                spectrumAbs = spectrumShifted.abs() / 10000;
                cv::imshow("Spectrum", array2image(spectrumAbs));
                cv::waitKey();
            }

        }
#endif

        static void runAllTests() {
            std::random_device rd;
            std::uniform_real_distribution<double> dist(0, 1);

            Eigen::ArrayXXcd spatial(64, 1);
            Eigen::ArrayXXcd spectral(64, 1);
            Eigen::ArrayXXcd spectralAnalytic(spatial.rows(), spatial.cols());

            for (int i = 0; i < spatial.rows(); i++) {
                spatial(i) = dist(rd);
            }

            FourierTransform fft(spatial);
            fft.compute(spatial, spectral);

            using namespace std::complex_literals;

            for (int i = 0; i < spatial.rows(); i++) {
                std::complex<double> sum = 0;
                for (int k = 0; k < spatial.rows(); k++) {
                    sum += spatial(k) * exp(-1i * (std::complex < double>)(2 * PI * k * i / spatial.rows()));
                }
                spectralAnalytic(i) = sum;
            }

            UNIT_TEST(areEqual(spectral, spectralAnalytic, 1e-12));
        }

        static double speed(unsigned long testCount) {
            Eigen::ArrayXXcd spatial = Eigen::ArrayXXcd::Random(512, 512);
            Eigen::ArrayXXcd spectral(spatial);

            FourierTransform ft(spatial);

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                ft.compute(spatial, spectral);
            }

            return toc(testCount);
        }
    };
}

#endif