/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "QRCodePatternCannyDetector.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    QRCodePatternCannyDetector::QRCodePatternCannyDetector()
    : PeriodicPatternDetector() {
    }

    QRCodePatternCannyDetector::QRCodePatternCannyDetector(double physicalPeriod, int size, int numberHalfPeriods, int lowCannyThreshold, int highCannyThreshold)
    : PeriodicPatternDetector() {
        resize(physicalPeriod, size, numberHalfPeriods, lowCannyThreshold, highCannyThreshold);
    }

    void QRCodePatternCannyDetector::resize(double physicalPeriod, int size, int numberHalfPeriods, int lowCannyThreshold, int highCannyThreshold) {
        //PeriodicPatternDetector::resize(size, size, 0, physicalPeriod, 0.005);
        setPixelPeriod(0);
        setPhysicalPeriod(physicalPeriod);
        PeriodicPatternDetector::resize(size, size);
        poseEstimation.resize(size, physicalPeriod, numberHalfPeriods);
        poseEstimation.detector.markerDetector.lowCannyThreshold = lowCannyThreshold;
        poseEstimation.detector.markerDetector.highCannyThreshold = highCannyThreshold;
    }

    void QRCodePatternCannyDetector::compute(cv::Mat image) {
        poseEstimation.compute(image);

        poseEstimation.drawPose(image);
        cv::imshow("image", image);

        poseEstimation.drawSnapshot();

        this->multiplePoses = poseEstimation.codes;
    }
#endif //USE_OPENCV
}