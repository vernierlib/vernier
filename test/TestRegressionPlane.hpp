/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTREGRESSIONPLANE_HPP
#define TESTREGRESSIONPLANE_HPP

#include "Vernier.hpp"
#include "eigen-matio/MatioFile.hpp"

namespace Vernier {

    class TestRegressionPlane {
    public:

        static void main1() {
            Eigen::ArrayXXd unwrappedSimple = Eigen::RowVectorXd::LinSpaced(10, 0, 18).replicate(8, 1);
            double cropFactor = 0.5;

            std::cout << unwrappedSimple << std::endl;
            RegressionPlane regressor(cropFactor);

            Plane plane = regressor.compute(unwrappedSimple);

            std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;
        }

        static void main2() {
            Eigen::ArrayXXd unwrappedSimple = Eigen::RowVectorXd::LinSpaced(10, 0, 18).replicate(8, 1).transpose();
            double cropFactor = 0.5;

            std::cout << unwrappedSimple << std::endl;
            RegressionPlane regressor(cropFactor);

            Plane plane = regressor.compute(unwrappedSimple);

            std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;
        }

        static void main3() { // deprecated
            int sideOffset = 250;
            Eigen::ArrayXXd unwrapCenterMatlab(1, 1);
            Eigen::MatioFile file("testfiles/TestFilesMat.mat", MAT_ACC_RDONLY);
            file.read_mat("unwrapCenterMatlab", unwrapCenterMatlab);

            Eigen::Vector3d coefficients;

            Eigen::ArrayXXd intermediaryMatrix;
            intermediaryMatrix = unwrapCenterMatlab.block(sideOffset, sideOffset, unwrapCenterMatlab.rows() - 2 * sideOffset, unwrapCenterMatlab.cols() - 2 * sideOffset);
            RegressionPlane leastSquaresPlane;

            Plane plane = leastSquaresPlane.compute(intermediaryMatrix);

            coefficients = plane.getCoefficients();

            Eigen::MatrixXd coeffMatrix(3, 1);
            coeffMatrix << coefficients(0), coefficients(1), coefficients(2);

            Eigen::MatrixXd planeCoeffRef(1, 1);
            file.read_mat("planeCoefficients", planeCoeffRef);

            std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;
            std::cout << "mat coefficients:  \n" << planeCoeffRef << std::endl;
            std::cout << "mat mean:  \n" << unwrapCenterMatlab(384, 512) << std::endl;

        }

        static void runAllTests() {
            Eigen::ArrayXXd unwrappedSimple = Eigen::RowVectorXd::LinSpaced(10, 0, 18).replicate(8, 1);

            RegressionPlane regressor(0.5);

            Plane plane = regressor.compute(unwrappedSimple);
            Eigen::MatrixXd planeCoeffRef(3, 1);
            planeCoeffRef << 2, 0, 10;
            Eigen::MatrixXd planeCoeff(3, 1);
            planeCoeff << plane.getA(), plane.getB(), plane.getC();

            std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;

            UNIT_TEST(areEqual(planeCoeffRef, planeCoeff));

            unwrappedSimple = Eigen::RowVectorXd::LinSpaced(10, 0, 18).replicate(8, 1).transpose();
            plane = regressor.compute(unwrappedSimple);
            planeCoeffRef << 0, 2, 10;
            planeCoeff << plane.getA(), plane.getB(), plane.getC();

            std::cout << "plane coefficients:  \n" << plane.toString() << std::endl;

            UNIT_TEST(areEqual(planeCoeffRef, planeCoeff));
        }

        static void test3() { // deprecated

            Eigen::ArrayXXd unwrapCenterMatlab(1, 1);
            Eigen::MatioFile file("testfiles/TestFilesMat.mat", MAT_ACC_RDONLY);
            file.read_mat("unwrapCenterMatlab", unwrapCenterMatlab);

            int sideOffset = 250;
            Eigen::ArrayXXd intermediaryMatrix;
            intermediaryMatrix = unwrapCenterMatlab.block(sideOffset, sideOffset, unwrapCenterMatlab.rows() - 2 * sideOffset, unwrapCenterMatlab.cols() - 2 * sideOffset);
            RegressionPlane leastSquaresPlane; //(intermediaryMatrix.cols(), intermediaryMatrix.rows());

            Plane plane = leastSquaresPlane.compute(intermediaryMatrix);

            Eigen::Vector3d coefficients;
            coefficients = plane.getCoefficients();

            Eigen::MatrixXd coeffMatrix(3, 1);
            coeffMatrix << coefficients(0), coefficients(1), coefficients(2);

            Eigen::MatrixXd planeCoeffRef(1, 1);
            file.read_mat("planeCoefficients", planeCoeffRef);

            UNIT_TEST(areEqual(planeCoeffRef, coeffMatrix));
        }

        static double speed(unsigned long testCount) {
            Eigen::ArrayXXd unwrap2(1, 1);
            int sideOffset = 250;
            Eigen::MatioFile file("testfiles/TestFilesMat.mat", MAT_ACC_RDONLY);
            file.read_mat("unwrapCenterMatlab", unwrap2);

            Eigen::Vector3d coefficients;

            RegressionPlane leastSquaresPlane;
            Eigen::ArrayXXd matTest;
            matTest = unwrap2.block(sideOffset - 1, sideOffset - 1, unwrap2.rows() - 2 * sideOffset + 1, unwrap2.cols() - 2 * sideOffset + 1);
            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                Plane plan = leastSquaresPlane.compute(matTest);
            }

            return toc(testCount);
        }
    };
}
#endif