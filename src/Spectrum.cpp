/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "Spectrum.hpp"

namespace vernier {

    void shift(Eigen::ArrayXXcd& source, Eigen::ArrayXXcd& dest) {
        ASSERT(source.rows() > 0 && source.cols() > 0);
        dest.resize(source.rows(), source.cols());
        int nTop = source.rows() / 2;
        int nBottom = source.rows() - nTop;
        int nLeft = source.cols() / 2;
        int nRight = source.cols() - nLeft;
        dest.block(0, 0, nTop, nLeft) = source.block(nBottom, nRight, nTop, nLeft);
        dest.block(nTop, nLeft, nBottom, nRight) = source.block(0, 0, nBottom, nRight);
        dest.block(nTop, 0, nBottom, nLeft) = source.block(0, nRight, nBottom, nLeft);
        dest.block(0, nLeft, nTop, nRight) = source.block(nBottom, 0, nTop, nRight);
    }

    void mainPeakHalfPlane(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2) {
        int offsetMin = source.rows() / 100.0; // MAGIC NUMBER
        if (offsetMin < 20)
            offsetMin = 20;

        source.block(source.rows() / 2 - offsetMin / 2, source.cols() / 2 - offsetMin / 2, offsetMin, offsetMin) = 0;

        double maxValue = -1.0;
        std::complex<double> complexValue;

        mainPeak1.setConstant(-1);
        mainPeak2.setConstant(-1);

        for (int col = 1; col < source.cols() - 1; col++) {
            for (int row = source.rows() / 2; row < source.rows() - 1; row++) {
                //complexValue = source(row, col);
                //norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
                double norm = std::abs(source(row, col)) + std::abs(source(row - 1, col)) + std::abs(source(row, col - 1)) + std::abs(source(row + 1, col)) + std::abs(source(row, col + 1));
                if (norm > maxValue) {
                    maxValue = norm;
                    mainPeak1.x() = col;
                    mainPeak1.y() = row;
                    mainPeak1.z() = norm / source.cols() / source.rows() / 5; // MAGIC NUMBER
                }
            }
        }

        //std::cout<< "Peak value (half plane method) : " << mainPeak1.z() <<std::endl;
        if (mainPeak1.z() > 1e-5) { // MAGIC NUMBER
            int PEAK_MASK = 8;
            source.block(mainPeak1.y() - PEAK_MASK, mainPeak1.x() - PEAK_MASK, 2 * PEAK_MASK, 2 * PEAK_MASK) = 0;
            source.block((source.rows() - mainPeak1.y()) - PEAK_MASK, (source.cols() - mainPeak1.x()) - PEAK_MASK, 2 * PEAK_MASK, 2 * PEAK_MASK) = 0;

            maxValue = -1;

            for (int col = 1; col < source.cols() - 1; col++) {
                for (int row = source.rows() / 2; row < source.rows() - 1; row++) {
                    //complexValue = source(row, col);
                    //norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
                    double norm = std::abs(source(row, col)) + std::abs(source(row - 1, col)) + std::abs(source(row, col - 1)) + std::abs(source(row + 1, col)) + std::abs(source(row, col + 1));
                    if (norm > maxValue) {
                        maxValue = norm;
                        mainPeak2.x() = col;
                        mainPeak2.y() = row;
                        mainPeak2.z() = norm / source.cols() / source.rows() / 5;
                    }
                }
            }
            
            if (mainPeak1.x() < mainPeak2.x()) {
                std::swap(mainPeak1, mainPeak2);
            }

        }

        //        if (mainPeak1.z() < 1e-5 || mainPeak2.z() < 1e-5) {
        //            throw Exception("Houston, we have a problem here, the first peak has not be found.");
        //        }


        //        std::cout<<"Image size"<<source.rows()<<"x"<<source.cols()<<std::endl;
        //        
        //        std::cout<<"Peak 1"<<mainPeak1<<std::endl;
        //        std::cout<<"Peak 2"<<mainPeak2<<std::endl;
        //        
    }

}