/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "RegressionPlane.hpp"

namespace Vernier {

    RegressionPlane::RegressionPlane() {
        colOffset = 0;
        rowOffset = 0;
        cropFactor = 0.5;
    }

    RegressionPlane::RegressionPlane(double cropFactor) : RegressionPlane() {
        setCropFactor(cropFactor);
    }

    void RegressionPlane::resize(int rows, int cols) {
        if (rows <= 0 || cols <= 0) {
            throw Exception("Can't resize a RegressionPlane with rows<=0 or cols<=0");
        } else if (getNRows() != rows || getNCols() != cols) {

            colOffset = (int) (cols * cropFactor / 2.0);
            rowOffset = (int) (rows * cropFactor / 2.0);
            cols -= 2 * colOffset;
            rows -= 2 * rowOffset;

            phaseCropped.resize(rows, cols);

            meshRow = Eigen::VectorXd::LinSpaced(rows, -rows / 2, rows / 2 - 1).replicate(1, cols);
            meshCol = Eigen::RowVectorXd::LinSpaced(cols, -cols / 2, cols / 2 - 1).replicate(rows, 1);

            matMean << meshCol.cwiseProduct(meshCol).mean(), meshCol.cwiseProduct(meshRow).mean(), meshCol.mean(),
                    meshCol.cwiseProduct(meshRow).mean(), meshRow.cwiseProduct(meshRow).mean(), meshRow.mean(),
                    meshCol.mean(), meshRow.mean(), 1;
        }
    }

    Plane RegressionPlane::compute(const Eigen::ArrayXXd & unwrappedPhase) {
        resize(unwrappedPhase.rows(), unwrappedPhase.cols());
        Eigen::Vector3d planeCoefficients;
        Eigen::Vector3d vecMean;

        phaseCropped = unwrappedPhase.block(rowOffset, colOffset, unwrappedPhase.rows() - 2 * rowOffset, unwrappedPhase.cols() - 2 * colOffset);

        vecMean.x() = meshCol.cwiseProduct(phaseCropped).mean();
        vecMean.y() = meshRow.cwiseProduct(phaseCropped).mean();
        vecMean.z() = phaseCropped.mean();

        planeCoefficients = matMean.inverse() * vecMean;
        return Plane(planeCoefficients);
    }

    void RegressionPlane::setCropFactor(double cropFactor) {
        if (cropFactor < 0 || cropFactor >= 1.0) {
            throw Exception("Can't resize a RegressionPlane with cropFactor<0 or cropFactor>=1.0");
        } else {
            this->cropFactor = cropFactor;
        }
    }

    int RegressionPlane::getColOffset() {
        return colOffset;
    }

    int RegressionPlane::getRowOffset() {
        return rowOffset;
    }

    int RegressionPlane::getNRows() {
        return meshRow.rows() + 2 * rowOffset;
    }

    int RegressionPlane::getNCols() {
        return meshRow.cols() + 2 * colOffset;
    }

    int RegressionPlane::getNRowsCropped() {
        return meshRow.rows();
    }

    int RegressionPlane::getNColsCropped() {
        return meshRow.cols();
    }


}