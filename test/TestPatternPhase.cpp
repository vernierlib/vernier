/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "PatternPhase.hpp"
#include "UnitTest.hpp"
#include "PeriodicPatternLayout.hpp"
#include "eigen-matio/MatioFile.hpp"
#include <random>

using namespace vernier;
using namespace std;

void main1() {

    cv::Mat image = cv::imread("data/im_1.png", 0);
    imshow("Image", image);

    PatternPhase phaseRetrieving;
    phaseRetrieving.setSigma(1);
    phaseRetrieving.compute(image);

    //            std::cout << "phase at the center : \n" << phaseRetrieving.getPhase1()(768/2,1024/2) << std::endl;

    std::cout << "planes coefficients : \n" << phaseRetrieving.getPlane1().toString() << std::endl;
    arrayShow("Plane 1", phaseRetrieving.getUnwrappedPhase1());
    arrayShow("Plane 2", phaseRetrieving.getUnwrappedPhase2());
    //            imshow("spectrumFiltered1", array2image(phaseRetrieving.spectrumFiltered1, 0.00001 ));
    //            imshow("spectrumFiltered2", array2image(phaseRetrieving.spectrumFiltered2, 0.00001 ));
    //            imshow("Spectrum", array2image(phaseRetrieving.spectrumShifted, 0.00001));
    cv::imshow("Spectrum GUI", phaseRetrieving.getPeaksImage());
    cv::waitKey();

}

void runAllTests() {

    START_UNIT_TEST;

    double period = 10.0;
    PeriodicPatternLayout layout(period, 81, 81);
    double x = 4.0;
    double y = 3.0;
    double alpha = 0.2;
    double pixelSize = 1.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    Eigen::ArrayXXd array(512, 512);
    layout.renderOrthographicProjection(patternPose, array);
    PatternPhase patternPhase;
    patternPhase.setSigma(1);
    patternPhase.compute(array);
    
    

    //            std::cout << "Plane 1 position : \n" << -patternPhase.getPlane1().getPosition(period) << std::endl;
    //            std::cout << "Plane 2 position : \n" << -patternPhase.getPlane2().getPosition(period) << std::endl;
    //            std::cout << "Plane 1 angle : \n" << patternPhase.getPlane1().getAngle() << std::endl;
    //            std::cout << "Plane 2 angle : \n" << patternPhase.getPlane2().getAngle() - PI / 2 << std::endl;
    
    //patternPhase.showControlImages();
    //cv::waitKey();

    UNIT_TEST(areEqual(x, -patternPhase.getPlane1().getPosition(period), 0.001));

    UNIT_TEST(areEqual(y, -patternPhase.getPlane2().getPosition(period), 0.001));

    UNIT_TEST(areEqual(alpha, patternPhase.getPlane1().getAngle(), 0.001));

}

void runAllTests2() {

    Eigen::ArrayXXcd mireMatrix;
    Eigen::MatrixXd planeCoefficientsReference(1, 1);

    Eigen::MatioFile file("data/TestFilesMat.mat", MAT_ACC_RDONLY);
    file.read_mat("planeCoefficients", planeCoefficientsReference);

    PatternPhase phaseRetrieving;
    phaseRetrieving.setSigma(5);

    cv::Mat image = cv::imread("data/im_1.png", 0);
    phaseRetrieving.compute(image);
    Eigen::MatrixXd coeffMatrix(3, 1);

    Eigen::Vector3d planeCoefficients;
    planeCoefficients << phaseRetrieving.getPlane1().getA(), phaseRetrieving.getPlane1().getB(), phaseRetrieving.getPlane1().getC();
    coeffMatrix << planeCoefficients(0), planeCoefficients(1), planeCoefficients(2);

    std::cout << "plane coefficients:  \n" << planeCoefficientsReference << std::endl;
    std::cout << "found coefficients:  \n" << planeCoefficients << std::endl;


    UNIT_TEST(areEqual(coeffMatrix, planeCoefficientsReference));

}

double speed(unsigned long testCount) {

    std::random_device rd;
    std::uniform_real_distribution<double> dist(8, 15);
    double period = dist(rd);
    int size = 1024;

    PeriodicPatternLayout layout(period / 2.0, 437, 437);

    Eigen::ArrayXXd array(size, size);

    std::uniform_real_distribution<double> dist2(-period / 2.0, period / 2.0);
    std::uniform_real_distribution<double> dist3(0, PI * 2.0);

    double xPosition = dist2(rd);
    double yPosition = dist2(rd);
    double alphaOrientation = dist3(rd);

    layout.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0, 1.0), array);

    PatternPhase phaseRetrieving(size, size);
    phaseRetrieving.setSigma(0.01 * 1024);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        phaseRetrieving.compute(array);
    }
    return toc(testCount);
}

int main(int argc, char** argv) {

    runAllTests();
    //runAllTests2();

    return EXIT_SUCCESS;
}