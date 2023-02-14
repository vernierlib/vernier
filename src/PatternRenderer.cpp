/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "PatternRenderer.hpp"

namespace Vernier {

    PatternRenderer::PatternRenderer(PatternLayout* patternLayout, double pixelSize, double focalLength) {
        if (patternLayout == NULL) {
            throw Exception("The pattern layout is NULL.");
        }
        this->patternLayout = patternLayout;
        this->pixelSize = pixelSize;
        this->focalLength = focalLength;
    }

    void PatternRenderer::renderOrthographicProjection(Eigen::Matrix4d cTp, Eigen::ArrayXXd & outputImage) {
        if (outputImage.rows() <= 0 || outputImage.rows() % 2 == 1) {
            throw Exception("The number of rows must be positive and even.");
        }
        if (outputImage.cols() <= 0 || outputImage.cols() % 2 == 1) {
            throw Exception("The number of columns must be positive and even.");
        }

        double cx = outputImage.cols() / 2.0;
        double cy = outputImage.rows() / 2.0;

        Eigen::MatrixXd cameraMatrix(3, 4);
        cameraMatrix << 1 / pixelSize, 0, 0, cx,
                0, 1 / pixelSize, 0, cy,
                0, 0, 0, 1;

        //Eigen::Matrix4d cTp = pose.getCameraPatternTransformation();

        Eigen::MatrixXd M(4, 3);
        M << 1, 0, 0,
                0, 1, 0,
                0, 0, 0,
                0, 0, 1;

        Eigen::Matrix3d projectionMatrix = cameraMatrix * cTp * M;
        Eigen::Matrix3d inverseMatrix = projectionMatrix.inverse();

        for (int col = 0; col < outputImage.cols(); col++) {
            for (int row = 0; row < outputImage.rows(); row++) {
                Eigen::Vector3d pointImage(col + 0.5, row + 0.5, 1);
                Eigen::Vector3d pointPattern = inverseMatrix * pointImage;
                outputImage(row, col) = patternLayout->getIntensity(pointPattern.x(), pointPattern.y());
            }
        }
    }

    void PatternRenderer::renderOrthographicProjection(Pose pose, Eigen::ArrayXXd & outputImage) {
        if (outputImage.rows() <= 0 || outputImage.rows() % 2 == 1) {
            throw Exception("The number of rows must be positive and even.");
        }
        if (outputImage.cols() <= 0 || outputImage.cols() % 2 == 1) {
            throw Exception("The number of columns must be positive and even.");
        }

        double cx = outputImage.cols() / 2.0;
        double cy = outputImage.rows() / 2.0;

        Eigen::MatrixXd cameraMatrix(3, 4);
        cameraMatrix << 1 / pixelSize, 0, 0, cx,
                0, 1 / pixelSize, 0, cy,
                0, 0, 0, 1;

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
                Eigen::Vector3d pointImage(col + 0.5, row + 0.5, 1);
                Eigen::Vector3d pointPattern = inverseMatrix * pointImage;
                outputImage(row, col) = patternLayout->getIntensity(pointPattern.x(), pointPattern.y());
            }
        }
    }

    void PatternRenderer::renderPerspectiveProjection(Eigen::Matrix4d cTp, Eigen::ArrayXXd & outputImage) {

        double cx = outputImage.cols() / 2.0;
        double cy = outputImage.rows() / 2.0;

        Eigen::MatrixXd cameraMatrix(3, 4);
        cameraMatrix << focalLength / pixelSize, 0, cx, 0,
                0, focalLength / pixelSize, cy, 0,
                0, 0, 1, 0;

        //Eigen::Matrix4d cTp = pose.getCameraPatternTransformation();

        Eigen::MatrixXd M(4, 3);
        M << 1, 0, 0,
                0, 1, 0,
                0, 0, 0,
                0, 0, 1;

        Eigen::Matrix3d projectionMatrix = cameraMatrix * cTp * M;
        Eigen::Matrix3d inverseMatrix = projectionMatrix.inverse();

        for (int col = 0; col < outputImage.cols(); col++) {
            for (int row = 0; row < outputImage.rows(); row++) {
                Eigen::Vector3d pointImage(col + 0.5, row + 0.5, 1);
                Eigen::Vector3d pointPattern = inverseMatrix * pointImage;
                pointPattern /= pointPattern.z();
                outputImage(row, col) = patternLayout->getIntensity(pointPattern.x(), pointPattern.y());
            }
        }
    }

    void PatternRenderer::renderPerspectiveProjection(Pose pose, Eigen::ArrayXXd & outputImage) {

        double cx = outputImage.cols() / 2.0;
        double cy = outputImage.rows() / 2.0;

        Eigen::MatrixXd cameraMatrix(3, 4);
        cameraMatrix << focalLength / pixelSize, 0, cx, 0,
                0, focalLength / pixelSize, cy, 0,
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
                Eigen::Vector3d pointImage(col + 0.5, row + 0.5, 1);
                Eigen::Vector3d pointPattern = inverseMatrix * pointImage;
                pointPattern /= pointPattern.z();
                outputImage(row, col) = patternLayout->getIntensity(pointPattern.x(), pointPattern.y());
            }
        }
    }

}