/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "RegressionPlane.hpp"
#include "UnitTest.hpp"
#include "eigen-matio/MatioFile.hpp"

using namespace vernier;
using namespace std;

void main1() {
    Eigen::ArrayXXd unwrappedSimple = Eigen::RowVectorXd::LinSpaced(10, 0, 18).replicate(8, 1);
    double cropFactor = 0.5;

    std::cout << unwrappedSimple << std::endl;
    RegressionPlane regressor(cropFactor);

    PhasePlane plane = regressor.compute(unwrappedSimple);

    std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;
}

void main2() {
    Eigen::ArrayXXd unwrappedSimple = Eigen::RowVectorXd::LinSpaced(10, 0, 18).replicate(8, 1).transpose();
    double cropFactor = 0.5;

    std::cout << unwrappedSimple << std::endl;
    RegressionPlane regressor(cropFactor);

    PhasePlane plane = regressor.compute(unwrappedSimple);

    std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;
}

void main3() { // the coefficients in the mat file are wrong !!!!!
    int sideOffset = 250;
    Eigen::ArrayXXd unwrapCenterMatlab(1, 1);
    Eigen::MatioFile file("data/TestFilesMat.mat", MAT_ACC_RDONLY);
    file.read_mat("unwrapCenterMatlab", unwrapCenterMatlab);

    Eigen::Vector3d coefficients;

    Eigen::ArrayXXd intermediaryMatrix;
    intermediaryMatrix = unwrapCenterMatlab.block(sideOffset, sideOffset, unwrapCenterMatlab.rows() - 2 * sideOffset, unwrapCenterMatlab.cols() - 2 * sideOffset);
    RegressionPlane leastSquaresPlane;

    PhasePlane plane = leastSquaresPlane.compute(intermediaryMatrix);

    coefficients = plane.getCoefficients();

    Eigen::MatrixXd coeffMatrix(3, 1);
    coeffMatrix << coefficients(0), coefficients(1), coefficients(2);

    Eigen::MatrixXd planeCoeffRef(1, 1);
    file.read_mat("planeCoefficients", planeCoeffRef);

    std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;
    std::cout << "mat coefficients:  \n" << planeCoeffRef << std::endl;
    std::cout << "mat mean:  \n" << unwrapCenterMatlab(384, 512) << std::endl;

}

void test() {
    
    START_UNIT_TEST;
    
    Eigen::ArrayXXd unwrappedSimple = Eigen::RowVectorXd::LinSpaced(10, 0, 18).replicate(8, 1);

    RegressionPlane regressor(0.5);

    PhasePlane plane = regressor.compute(unwrappedSimple);
    Eigen::MatrixXd planeCoeffRef(3, 1);
    planeCoeffRef << 2, 0, 10;
    Eigen::MatrixXd planeCoeff(3, 1);
    planeCoeff << plane.getA(), plane.getB(), plane.getC();

    //std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;

    UNIT_TEST(areEqual(planeCoeffRef, planeCoeff));

    unwrappedSimple = Eigen::RowVectorXd::LinSpaced(10, 0, 18).replicate(8, 1).transpose();
    plane = regressor.compute(unwrappedSimple);
    planeCoeffRef << 0, 2, 10;
    planeCoeff << plane.getA(), plane.getB(), plane.getC();

    //std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;

    UNIT_TEST(areEqual(planeCoeffRef, planeCoeff));
}

double speed(unsigned long testCount) {
    Eigen::ArrayXXd unwrap2(1, 1);
    int sideOffset = 250;
    Eigen::MatioFile file("data/TestFilesMat.mat", MAT_ACC_RDONLY);
    file.read_mat("unwrapCenterMatlab", unwrap2);

    Eigen::Vector3d coefficients;

    RegressionPlane leastSquaresPlane;
    Eigen::ArrayXXd matTest;
    matTest = unwrap2.block(sideOffset - 1, sideOffset - 1, unwrap2.rows() - 2 * sideOffset + 1, unwrap2.cols() - 2 * sideOffset + 1);
    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        PhasePlane plan = leastSquaresPlane.compute(matTest);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {

    test();

    return EXIT_SUCCESS;
}