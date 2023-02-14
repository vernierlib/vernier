/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "QRCodePatternDetector.hpp"

namespace Vernier {

    void QRCodePatternDetector::createFinderPattern() {
        finderPattern.resize(7, 7);
        finderPattern << 1, 1, 1, 1, 1, 1, 1,
                1, 0, 0, 0, 0, 0, 1,
                1, 0, 1, 1, 1, 0, 1,
                1, 0, 1, 1, 1, 0, 1,
                1, 0, 1, 1, 1, 0, 1,
                1, 0, 0, 0, 0, 0, 1,
                1, 1, 1, 1, 1, 1, 1;
    }

    void QRCodePatternDetector::readJSON(rapidjson::Value& document) {
        PatternDetector::readJSON(document);
        if (document.HasMember("period") && document["period"].IsDouble()) {
            physicalPeriod = document["period"].GetDouble();
            PeriodicPatternDetector::setPhysicalPeriod(physicalPeriod);
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the period is missing or has a wrong format.");
        }
        if (document.HasMember("size") && document["size"].IsInt()) {
            qrCodeSize = document["size"].GetInt();
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the period is missing or has a wrong format.");
        }
        resize(qrCodeSize, 0, physicalPeriod);

//        if (document.HasMember("approxPixelPeriod") && document["approxPixelPeriod"].IsDouble()) {
//            this->approxPixelPeriod = document["approxPixelPeriod"].GetDouble();
//            PeriodicPatternDetector::setApproxPixelPeriod(document["approxPixelPeriod"].GetDouble());
//        }


        //if (document.HasMember("sigmaFilter") && document["sigmaFilter"].IsDouble()) {
        //    this->sigmaPercent = document["sigmaFilter"].GetDouble();
        //}
        //else {
        //    this->sigmaPercent = 0.005;
        //}
    }

    QRCodePatternDetector::QRCodePatternDetector() : QRCodePatternDetector(0, 0.0, 0.0) {
    }

    QRCodePatternDetector::QRCodePatternDetector(int qrCodeSize, double approxPixelPeriod, double physicalPeriod)
    : BitmapPatternDetector(Eigen::ArrayXXd::Zero(qrCodeSize, qrCodeSize), 0.0, 0.0) {
        resize(qrCodeSize, approxPixelPeriod, physicalPeriod);
    }

    void QRCodePatternDetector::resize(int qrCodeSize, double approxPixelPeriod, double physicalPeriod) {
        bitmapKernel.resize(qrCodeSize + 2, qrCodeSize + 2);
        bitmapKernel.setZero();

        createFinderPattern();

        if (qrCodeSize > 14) {
            for (int i = 0; i < bitmapKernel.rows() - 1; i++) {
                for (int j = 0; j < bitmapKernel.cols() - 1; j++) {
                    if (i % 2 == 1 && j % 2 == 1) {
                        bitmapKernel(i, j) = 1;
                    } else {
                        bitmapKernel(i, j) = 0;
                    }
                }
            }

            bitmapKernel.block(1, 1, 7, 7) = finderPattern;
            bitmapKernel.block(bitmapKernel.rows() - 8, 1, 7, 7) = finderPattern;
            bitmapKernel.block(1, bitmapKernel.cols() - 8, 7, 7) = finderPattern;
        }

        if (qrCodeSize % 4 != 1) {
            isInverted = true;
        } else {
            isInverted = false;
        }

        BitmapPatternDetector::resize(bitmapKernel, approxPixelPeriod, physicalPeriod);
    }

    void QRCodePatternDetector::compute(Eigen::ArrayXXd& pattern) {
        BitmapPatternDetector::compute(pattern);
    }

#ifdef USE_OPENCV

    void QRCodePatternDetector::computePerspective(Eigen::ArrayXXd& pattern) {
        BitmapPatternDetector::computePerspective(pattern);
    }
#endif // USE_OPENCV

    Pose QRCodePatternDetector::getCameraPoseInPattern() {
        return BitmapPatternDetector::getCameraPoseInPattern();
    }

    Pose QRCodePatternDetector::getPatternPoseInCamera() {
        return BitmapPatternDetector::getPatternPoseInCamera();
    }

    std::vector<Pose> QRCodePatternDetector::getAll3DPoses() {
        return BitmapPatternDetector::getAll3DPoses();
    }
}