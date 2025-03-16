/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Vernier.hpp"
#include "UnitTest.hpp"
#include <complex>
#include <random>

using namespace vernier;
using namespace std;

void main1() {
    Eigen::ArrayXXcd spatial(8, 4);
    Eigen::ArrayXXcd spectral(8, 4);

    spatial << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32;

    FourierTransform fft(8, 4, FFTW_FORWARD);
    std::cout << "spatial = " << spatial << std::endl;

    fft.compute(spatial, spectral);

    Eigen::ArrayXXcd spectralShifted(spectral.rows(), spectral.cols());
    Spectrum::shift(spectral, spectralShifted);

    std::cout << "spectral = " << spectral << std::endl;
    std::cout << "spectral shifted = " << spectralShifted << std::endl;

    FourierTransform ifft(8, 4, FFTW_BACKWARD);

    fft.compute(spectral, spatial);

    std::cout << "spatial = " << spatial << std::endl;

}

void main2() {

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
        cv::Mat image;
        array2image8UC4(spatial, image);
        cv::imshow("Spatial", image);

        // Fourier transform
        spatial.resize(array.rows(), array.cols());
        spatial.real() = array;
        ft.compute(spatial, spectrum);
        Spectrum::shift(spectrum, spectrumShifted);

        // Showing spectrum
        spectrumAbs = spectrumShifted.abs() / 10000;
        array2image8UC4(spectrumAbs, image);
        cv::imshow("Spectrum", image);
        cv::waitKey();
    }

}

void runAllTests() {

    START_UNIT_TEST;

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

    //    cout << spectral << endl <<"--------------" << endl;
    //    cout << spectralAnalytic << endl <<"--------------"<< endl;
    //    cout << spectral-spectralAnalytic << endl <<"--------------" << endl;


    UNIT_TEST(areEqual(spectral, spectralAnalytic, 1e-12));
}

double speed(unsigned long testCount) {
    Eigen::ArrayXXcd spatial = Eigen::ArrayXXcd::Random(512, 512);
    Eigen::ArrayXXcd spectral(spatial);

    FourierTransform ft(spatial);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        ft.compute(spatial, spectral);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {

    main1();

    runAllTests();

    return EXIT_SUCCESS;
}