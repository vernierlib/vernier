/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "PatternLayout.hpp"

namespace Vernier {

    class Camera {
    private:

        double lensRadius;
        double focalDistance;
        double lensToSensorDistance;
        double pixelSize;
        int ncols;
        int nrows;
        int bitDepth = 8;
        Eigen::Vector2d principalPoint;

    public:

        Camera() {

        }

        /** Renders an image with ray tracing algorithm
         *              
         *  \param pose: pattern pose (defines cTp and the scale factor)
         *  \param outputImage: any size double array 
         * 
         */

        void render(Layout * layout, const Eigen::Matrix4d & cameraToObjectTransform, Eigen::ArrayXXd & outputImage) {

            if (outputImage.rows() <= 0 || outputImage.rows() % 2 == 1) {
                throw Exception("The number of rows must be positive and even.");
            }
            if (outputImage.cols() <= 0 || outputImage.cols() % 2 == 1) {
                throw Exception("The number of columns must be positive and even.");
            }

            if (principalPoint(0) < 0) {
                principalPoint(0) = outputImage.cols() / 2.0;
                principalPoint(1) = outputImage.rows() / 2.0;
            }

            Eigen::MatrixXd cameraMatrix(3, 4);
            cameraMatrix << focalLength / pose.pixelSize, 0, principalPoint(0), 0,
                    0, focalLength / pose.pixelSize, principalPoint(1), 0,
                    0, 0, 1, 0;

            Eigen::Matrix4d cTp = pose.getCameraToPatternTransformationMatrix();

            Eigen::MatrixXd M(4, 3);
            M << 1, 0, 0,
                    0, 1, 0,
                    0, 0, 0,
                    0, 0, 1;

            Eigen::Matrix3d projectionMatrix = cameraMatrix * cTp * M;
            Eigen::Matrix3d inverseMatrix = projectionMatrix.inverse();

            for (int col = 0; col < outputImage.cols(); col++) {
                for (int row = 0; row < outputImage.rows(); row++) {
                    //Eigen::Vector3d pointImage(col + 0.5, row + 0.5, 1);
                    Eigen::Vector3d pointImage(col, row, 1);
                    Eigen::Vector3d pointPattern = inverseMatrix * pointImage;
                    pointPattern /= pointPattern.z();
                    outputImage(row, col) = this->getIntensity(pointPattern.x(), pointPattern.y());
                }
            }
        }


    };

}

#endif