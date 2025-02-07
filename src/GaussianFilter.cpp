/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "GaussianFilter.hpp"

namespace vernier {

    GaussianFilter::GaussianFilter() {
        sigma = 0.0;
    }

    GaussianFilter::GaussianFilter(double sigma) {
        setSigma(sigma);
    }

    void GaussianFilter::resize(int nRows, int nCols) {
        if (nRows <= 0 || nCols <= 0) {
            throw Exception("Can't resize a HyperGaussianFilter with rows<=0 or cols<=0");
        } else if (nRows != kernel.rows() || nCols != kernel.cols()) {
            kernel.resize(nRows, nCols);
            int kernelCenterRow = nRows / 2;
            int kernelCenterCol = nCols / 2;
            double sigma2 = 2 * sigma * sigma;
            for (int col = 0; col < kernel.cols(); col++) {
                double dCol = col - kernelCenterCol;
                for (int row = 0; row < kernel.rows(); row++) {
                    double dRow = row - kernelCenterRow;
                    double radius2 = dRow * dRow + dCol * dCol;
                    double coeff = exp(-radius2 / sigma2);
                    kernel(row, col) = coeff;
                }
            }
        }
    }

    void GaussianFilter::setSigma(double sigma) {
        if (sigma <= 0.0) {
            throw Exception("Can't construct a GaussianFilter with sigma<=0.0");
        } else {
            this->sigma = sigma;
            int kernelSize = 1 + 2 * (int) (sigma*sqrt(-2 * log(1e-16)) );
            resize(kernelSize, kernelSize);
        }
    }

    void GaussianFilter::applyTo(Eigen::ArrayXXcd& inputArray, int centerRow, int centerCol) {
        int kernelCenterRow = kernel.rows() / 2;
        int kernelCenterCol = kernel.cols() / 2;
        for (int col = 0; col < inputArray.cols(); col++) {
            int kernelCol = col - (centerCol - kernelCenterCol);
            if (kernelCol >= 0 && kernelCol < kernel.cols()) {
                for (int row = 0; row < inputArray.rows(); row++) {
                    int kernelRow = row - (centerRow - kernelCenterRow);
                    if (kernelRow >= 0 && kernelRow < kernel.rows()) {
                        inputArray(row, col) *= kernel(kernelRow, kernelCol);
                    } else {
                        inputArray(row, col) = 0.0;
                    }
                }
            } else {
                for (int row = 0; row < inputArray.rows(); row++) {
                    inputArray(row, col) = 0.0;
                }
            }
        }
    }

    void gaussianFilter(Eigen::ArrayXXcd& array, double centerRow, double centerCol, double sigma) {
        double sigma2 = 2 * sigma * sigma;
        for (int col = 0; col < array.cols(); col++) {
            double dCol = col - centerCol;
            for (int row = 0; row < array.rows(); row++) {
                double dRow = row - centerRow;
                double radius2 = dRow * dRow + dCol * dCol;
                double coeff = exp(-radius2 / sigma2);
                array(row, col) *= coeff;
            }
        }
    }
}