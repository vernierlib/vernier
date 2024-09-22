/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "Spectrum.hpp"
#include "GaussianFilter.hpp"

namespace Vernier {

    void Spectrum::shift(Eigen::ArrayXXcd& source, Eigen::ArrayXXcd& dest) {
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

    void Spectrum::mainPeakCircle(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2, double approxPixelPeriod) {
        double maxValue = 0;
        Eigen::ArrayXXcd sourceSave = source;

        int offset = (((double) source.rows() / (double) approxPixelPeriod)) * (sqrt(2) / 2);

        for (int col = source.cols() / 2 + offset; col < source.cols(); col++) {
            for (int row = source.rows() / 2 - offset; row < source.rows() / 2 + offset; row++) {
                std::complex<double> complexValue = source(row, col);


                double frequenceColApprox = ((double) col - (double) source.cols() / 2.0) / ((double) source.cols() / (double) approxPixelPeriod);
                double frequenceRowApprox = ((double) row - (double) source.rows() / 2.0) / ((double) source.rows() / (double) approxPixelPeriod);
                double frequenceColMin = frequenceColApprox - frequenceColApprox * (sqrt(2) - 1) / 2.0;
                double frequenceRowMin = frequenceRowApprox - frequenceRowApprox * (sqrt(2) - 1) / 2.0;
                double frequenceColMax = frequenceColApprox + frequenceColApprox * (sqrt(2) - 1) / 2.0;
                double frequenceRowMax = frequenceRowApprox + frequenceRowApprox * (sqrt(2) - 1) / 2.0;

                double radiusMin = sqrt(frequenceColMin * frequenceColMin + frequenceRowMin * frequenceRowMin);
                double radiusMax = sqrt(frequenceColMax * frequenceColMax + frequenceRowMax * frequenceRowMax);

                if (radiusMax >= 1 && radiusMin <= 1) {
                    double norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
                    if (norm > maxValue) {
                        maxValue = norm;
                        mainPeak1.x() = col;
                        mainPeak1.y() = row;
                        mainPeak1.z() = 1;
                    }
                    //source(row, col) = 255;
                }
            }
        }
        //std::cout<< "Peak value (circle method) : " << maxValue/source.cols()/source.rows() <<std::endl;
        
        Eigen::Matrix3d KTotal;

        KTotal << 0.0, -(double) source.cols() / (double) source.rows(), (double) source.cols(),
                (double) source.rows() / (double) source.cols(), 0.0, 0.0,
                0.0, 0.0, 1.0;

        Eigen::Vector3d mainPeak2supposed;

        mainPeak2supposed = KTotal * mainPeak1;

        maxValue = 0;

        ASSERT(mainPeak2supposed.x() - 4 > 0 && mainPeak2supposed.y() - 4 > 0);

        double fApprox = ((double) source.cols() / (double) approxPixelPeriod);
        double fMin = fApprox - fApprox * (sqrt(2) - 1) / 2.0;
        double fMax = fApprox + fApprox * (sqrt(2) - 1) / 2.0;
        int sizeSearchP2 = (int) (round(fMax - fMin));

        for (int col = (int) mainPeak2supposed.x() - sizeSearchP2; col < (int) mainPeak2supposed.x() + sizeSearchP2; col++) {
            for (int row = (int) mainPeak2supposed.y() - sizeSearchP2; row < (int) mainPeak2supposed.y() + sizeSearchP2; row++) {
                std::complex<double> complexValue2 = source(row, col);
                double norm2 = complexValue2.real() * complexValue2.real() + complexValue2.imag() * complexValue2.imag();
                if (norm2 > maxValue) {
                    maxValue = norm2;
                    mainPeak2.x() = col;
                    mainPeak2.y() = row;
                    mainPeak2.z() = 1;
                }
            }
        }

        source = sourceSave;

    }

    void Spectrum::mainPeakQuarter(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2) {
        int offsetMin = source.rows() / 100.0;
        source.block(source.rows() / 2 - offsetMin / 2, source.cols() / 2 - offsetMin / 2, offsetMin, offsetMin) = 0;

        double maxValue = 0;

        for (int col = source.cols() / 2 + 10; col < source.cols(); col++) {
            for (int row = source.rows() / 2 - 2; row < source.rows(); row++) {
                std::complex<double> complexValue = source(row, col);
                double norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
                if (norm > maxValue) {
                    maxValue = norm;
                    mainPeak1.x() = col;
                    mainPeak1.y() = row;
                    mainPeak1.z() = 1;
                }
            }
        }

        Eigen::Matrix3d KTotal;

        KTotal << 0.0, -(double) source.cols() / (double) source.rows(), (double) source.cols(),
                (double) source.rows() / (double) source.cols(), 0.0, 0.0,
                0.0, 0.0, 1.0;

        Eigen::Vector3d mainPeak2supposed;

        mainPeak2supposed = KTotal * mainPeak1;

        maxValue = 0;

        ASSERT(mainPeak2supposed.x() - 4 > 0 && mainPeak2supposed.y() - 4 > 0);

        double peak2Angle = atan2(mainPeak2supposed.x() - source.rows() / 2, mainPeak2supposed.y() - source.cols() / 2);

        double thetaMax = 45.0 * PI / 180.0;

        for (int row = -source.rows() / 2; row < source.rows() / 2; row++) {
            for (int col = -source.cols() / 2; col < source.cols() / 2; col++) {

                double r = sqrt(row * row + col * col);
                double rxMin = r * cos(peak2Angle - thetaMax) + source.rows() / 2;
                double ryMin = r * sin(peak2Angle - thetaMax) + source.cols() / 2;
                double rxMax = r * cos(peak2Angle + thetaMax) + source.rows() / 2;
                double ryMax = r * sin(peak2Angle + thetaMax) + source.cols() / 2;

                if ((row > rxMin - source.rows() / 2 && row < rxMax - source.rows() / 2 && col < ryMax - source.cols() / 2) || (row > rxMax - source.rows() / 2 && col > ryMin - source.cols() / 2 && col < ryMax - source.cols() / 2)) {
                    std::complex<double> complexValue2 = source(source.rows() / 2 + row, source.cols() / 2 + col);
                    double norm2 = complexValue2.real() * complexValue2.real() + complexValue2.imag() * complexValue2.imag();
                    if (norm2 > maxValue) {
                        maxValue = norm2;
                        mainPeak2.x() = source.rows() / 2 + col;
                        mainPeak2.y() = source.cols() / 2 + row;
                        mainPeak2.z() = 1;
                    }
                }

            }
        }
    }

    void Spectrum::mainPeakPerimeter(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2) {
        Eigen::ArrayXXcd sourceSave = source;
        int offsetMin = source.rows() / 100.0;
        source.block(source.rows() / 2 - offsetMin / 2, source.cols() / 2 - offsetMin / 2, offsetMin, offsetMin) = 0;

        Eigen::ArrayXXd mainPeakList(0, 3);

        int count = 0;

        while (mainPeakList.rows() < 8) {

            double maxValue = 0;
            mainPeakList.conservativeResize(mainPeakList.rows() + 1, 3);

            for (int col = 0; col < source.cols(); col++) {
                for (int row = 0; row < source.rows(); row++) {
                    std::complex<double> complexValue = source(row, col);
                    double norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
                    if (norm > maxValue) {
                        maxValue = norm;
                        mainPeakList(mainPeakList.rows() - 1, 0) = col;
                        mainPeakList(mainPeakList.rows() - 1, 1) = row;
                        mainPeakList(mainPeakList.rows() - 1, 2) = atan2(row - source.rows() / 2, col - source.cols() / 2);

                    }

                }

            }

            if (mainPeakList(mainPeakList.rows() - 1, 1) - 4 > 0 && mainPeakList(mainPeakList.rows() - 1, 0) - 4 > 0 && mainPeakList(mainPeakList.rows() - 1, 1) + 4 < source.rows() && mainPeakList(mainPeakList.rows() - 1, 0) + 4 < source.cols()) {
                source.block(mainPeakList(mainPeakList.rows() - 1, 1) - 4, mainPeakList(mainPeakList.rows() - 1, 0) - 4, 8, 8) = 0;

            } else {
                source(mainPeakList(mainPeakList.rows() - 1, 1), mainPeakList(mainPeakList.rows() - 1, 0)) = 0;

            }

            count++;

        }


        Eigen::ArrayXXd mainPeakListOrdered(mainPeakList.rows(), 3);
        for (int k = 0; k < mainPeakList.rows(); k++) {
            double minAngle = 10e10;
            int iterator = 0;
            for (int i = 0; i < mainPeakList.rows(); i++) {
                if (mainPeakList(i, 2) < minAngle) {
                    minAngle = mainPeakList(i, 2);
                    iterator = i;

                }

            }
            mainPeakListOrdered.block(k, 0, 1, 3) = mainPeakList.block(iterator, 0, 1, 3);
            mainPeakList(iterator, 2) = 10e6;

        }

        double dist1 = 0;
        double dist2 = 0;
        for (int i = 0; i < mainPeakListOrdered.rows() / 2 - 1; i++) {
            dist1 += getDistancePoints(mainPeakListOrdered(2 * i, 0), mainPeakListOrdered(2 * i, 1), mainPeakListOrdered(2 * (i + 1), 0), mainPeakListOrdered(2 * (i + 1), 1));
            dist2 += getDistancePoints(mainPeakListOrdered(2 * i + 1, 0), mainPeakListOrdered(2 * i + 1, 1), mainPeakListOrdered(2 * (i + 1) + 1, 0), mainPeakListOrdered(2 * (i + 1) + 1, 1));

        }

        if (dist1 < dist2) {
            mainPeak1.x() = mainPeakListOrdered(4, 0);
            mainPeak1.y() = mainPeakListOrdered(4, 1);
            mainPeak2.x() = mainPeakListOrdered(6, 0);
            mainPeak2.y() = mainPeakListOrdered(6, 1);

        } else {
            mainPeak1.x() = mainPeakListOrdered(3, 0);
            mainPeak1.y() = mainPeakListOrdered(3, 1);
            mainPeak2.x() = mainPeakListOrdered(5, 0);
            mainPeak2.y() = mainPeakListOrdered(5, 1);
        }

        source = sourceSave;

    }

    void Spectrum::mainPeakHalfPlane(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2) {
        int offsetMin = source.rows() / 100.0; // MAGIC NUMBER
        if (offsetMin < 20) 
            offsetMin = 20;
        source.block(source.rows() / 2 - offsetMin / 2, source.cols() / 2 - offsetMin / 2, offsetMin, offsetMin) = 0;

        double maxValue = -1.0;
        std::complex<double> complexValue;
        double norm = 0.0;

        for (int col = 1; col < source.cols()-1; col++) {
            for (int row = source.rows()/2; row < source.rows()-1; row++) {
                //complexValue = source(row, col);
                //norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
                norm = std::abs(source(row, col)) + std::abs(source(row - 1, col)) + std::abs(source(row, col - 1)) + std::abs(source(row + 1, col ))+ std::abs(source(row , col +1));
                if (norm > maxValue) {
                    maxValue = norm;
                    mainPeak1.x() = col;
                    mainPeak1.y() = row;
                    mainPeak1.z() = norm/source.cols()/source.rows()/5;
                }
            }
        }
        //std::cout<< "Peak value (half plane method) : " << mainPeak1.z() <<std::endl;
        if (mainPeak1.z() < 1e-5) { // MAGIC NUMBER
            //cv::imshow("Peaks not found", array2image(source) );
            //cv::waitKey();
            throw Exception("Houston, we have a problem here, the first peak has not be found.");
        }
        
        source.block(mainPeak1.y() - 4, mainPeak1.x() - 4, 8, 8) = 0;
        source.block((source.rows()-mainPeak1.y()) - 4, (source.cols()-mainPeak1.x()) - 4, 8, 8) = 0;

        maxValue = 0;

        for (int col = 1; col < source.cols()-1; col++) {
            for (int row = source.rows()/2; row < source.rows()-1; row++) {
                //complexValue = source(row, col);
                //norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
                norm = std::abs(source(row, col)) + std::abs(source(row - 1, col)) + std::abs(source(row, col - 1)) + std::abs(source(row + 1, col ))+ std::abs(source(row , col +1));
                if (norm > maxValue) {
                    maxValue = norm;
                    mainPeak2.x() = col;
                    mainPeak2.y() = row;
                    mainPeak2.z() = norm/source.cols()/source.rows()/5;
                }
            }
        }
        
        if (mainPeak1.x()<mainPeak2.x()) {
            std::swap(mainPeak1,mainPeak2);
        }
        
//        std::cout<<"Image size"<<source.rows()<<"x"<<source.cols()<<std::endl;
//        
//        std::cout<<"Peak 1"<<mainPeak1<<std::endl;
//        std::cout<<"Peak 2"<<mainPeak2<<std::endl;
//        
        
        
    }

    //    void Spectrum::mainPeakHalfPlane(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2) {
    //        int offsetMin = source.rows() / 100.0;
    //        source.block(source.rows() / 2 - offsetMin / 2, source.cols() / 2 - offsetMin / 2, offsetMin, offsetMin) = 0;
    //
    //        double maxValue = 0;
    //        std::complex<double> complexValue;
    //        double norm;
    //
    //        for (int col = source.cols() / 2 + 10; col < source.cols(); col++) {
    //            for (int row = source.rows() / 2 - 20; row < source.rows(); row++) {
    //                complexValue = source(row, col);
    //                norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
    //                if (norm > maxValue) {
    //                    maxValue = norm;
    //                    mainPeak1.x() = col;
    //                    mainPeak1.y() = row;
    //                    mainPeak1.z() = 1;
    //                }
    //            }
    //        }
    //        source.block(mainPeak1.y() - 4, mainPeak1.x() - 4, 8, 8) = 0;
    //
    //        maxValue = 0;
    //
    //        for (int col = 0; col < source.cols(); col++) {
    //            for (int row = source.rows() / 2 + 2; row < source.rows(); row++) {
    //                complexValue = source(row, col);
    //                norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
    //                if (norm > maxValue) {
    //                    maxValue = norm;
    //                    mainPeak2.x() = col;
    //                    mainPeak2.y() = row;
    //                    mainPeak2.z() = 1;
    //                }
    //            }
    //        }
    //    }

    void Spectrum::mainPeak4Search(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2) {

        Eigen::ArrayXXd mainPeakList(0, 4);

        Eigen::ArrayXXcd sourceSave = source;


        sourceSave.block(sourceSave.rows() / 2 - 4, sourceSave.cols() / 2 - 4, 8, 8) = 0;
        sourceSave = sourceSave / 50;

        int offsetMin = source.rows() / 100.0;
        source.block(source.rows() / 2 - offsetMin / 2, source.cols() / 2 - offsetMin / 2, offsetMin, offsetMin) = 0;

        int count = 0;

        while (mainPeakList.rows() < 10) {

            double maxValue = 0;
            mainPeakList.conservativeResize(mainPeakList.rows() + 1, mainPeakList.cols());

            for (int col = 0; col < source.cols(); col++) {
                for (int row = 0; row < source.rows(); row++) {
                    std::complex<double> complexValue = source(row, col);
                    double norm = complexValue.real() * complexValue.real() + complexValue.imag() * complexValue.imag();
                    if (norm > maxValue) {
                        maxValue = norm;
                        mainPeakList(mainPeakList.rows() - 1, 0) = col;
                        mainPeakList(mainPeakList.rows() - 1, 1) = row;
                        mainPeakList(mainPeakList.rows() - 1, 2) = atan2(row - source.rows() / 2, col - source.cols() / 2);
                    }
                }
            }

            double sum;
            if (mainPeakList(mainPeakList.rows() - 1, 1) - 1 > 0 && mainPeakList(mainPeakList.rows() - 1, 0) - 1 > 0 && mainPeakList(mainPeakList.rows() - 1, 1) + 1 < source.rows() && mainPeakList(mainPeakList.rows() - 1, 0) + 1 < source.cols()) {
                sum = source.block(mainPeakList(mainPeakList.rows() - 1, 1) - 1, mainPeakList(mainPeakList.rows() - 1, 0) - 1, 2, 2).abs().sum();
                mainPeakList(mainPeakList.rows() - 1, 3) = sum;
            } else {
                sum = 0;
                mainPeakList(mainPeakList.rows() - 1, 3) = sum;
            }

            if (mainPeakList(mainPeakList.rows() - 1, 1) - 4 > 0 && mainPeakList(mainPeakList.rows() - 1, 0) - 4 > 0 && mainPeakList(mainPeakList.rows() - 1, 1) + 4 < source.rows() && mainPeakList(mainPeakList.rows() - 1, 0) + 4 < source.cols()) {
                source.block(mainPeakList(mainPeakList.rows() - 1, 1) - 4, mainPeakList(mainPeakList.rows() - 1, 0) - 4, 8, 8) = 0;
            } else {
                source(mainPeakList(mainPeakList.rows() - 1, 1), mainPeakList(mainPeakList.rows() - 1, 0)) = 0;
            }

            count++;

        }

        Eigen::ArrayXXd mainPeakListOrderedMag(mainPeakList.rows(), mainPeakList.cols());
        for (int k = 0; k < mainPeakList.rows(); k++) {
            double maxSum = 0;
            int iterator = 0;
            for (int i = 0; i < mainPeakList.rows(); i++) {
                if (mainPeakList(i, 3) > maxSum) {
                    maxSum = mainPeakList(i, 3);
                    iterator = i;
                }
            }
            mainPeakListOrderedMag.block(k, 0, 1, mainPeakList.cols()) = mainPeakList.block(iterator, 0, 1, mainPeakList.cols());
            mainPeakList(iterator, 3) = 0;
        }

        Eigen::ArrayXXd mainPeakListReduced = mainPeakListOrderedMag.block(0, 0, 4, mainPeakListOrderedMag.cols());
        Eigen::ArrayXXd mainPeakListOrdered(mainPeakListReduced.rows(), mainPeakList.cols());
        for (int k = 0; k < mainPeakListReduced.rows(); k++) {
            double minAngle = 10e10;
            int iterator = 0;
            for (int i = 0; i < mainPeakListReduced.rows(); i++) {
                if (mainPeakListReduced(i, 2) < minAngle) {
                    minAngle = mainPeakListReduced(i, 2);
                    iterator = i;
                }
            }
            mainPeakListOrdered.block(k, 0, 1, mainPeakListReduced.cols()) = mainPeakListReduced.block(iterator, 0, 1, mainPeakList.cols());
            mainPeakListReduced(iterator, 2) = 10e6;
        }

        if (mainPeakListOrdered(3, 2) > 7.0 * PI / 8.0) {
            mainPeak1.x() = mainPeakListOrdered(1, 0);
            mainPeak1.y() = mainPeakListOrdered(1, 1);
            mainPeak2.x() = mainPeakListOrdered(2, 0);
            mainPeak2.y() = mainPeakListOrdered(2, 1);
        } else {
            mainPeak1.x() = mainPeakListOrdered(2, 0);
            mainPeak1.y() = mainPeakListOrdered(2, 1);
            mainPeak2.x() = mainPeakListOrdered(3, 0);
            mainPeak2.y() = mainPeakListOrdered(3, 1);
        }

        source = sourceSave;

    }

    double Spectrum::getDistancePoints(double x1, double y1, double x2, double y2) {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

}