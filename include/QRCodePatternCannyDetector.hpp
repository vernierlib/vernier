/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef QRCODEPATTERNCANNYDETECTOR_HPP
#define QRCODEPATTERNCANNYDETECTOR_HPP

#include "PeriodicPatternDetector.hpp"
#include "QRCodePoseEstimation.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    class QRCodePatternCannyDetector : public PeriodicPatternDetector {
    private:
        QRCodePoseEstimation poseEstimation;
        std::map<int, Pose> multiplePoses;

    public:
        QRCodePatternCannyDetector();

        QRCodePatternCannyDetector(double physicalPeriod, int size, int numberHalfPeriods, int lowCannyThreshold, int highCannyThreshold);

        void resize(double physicalPeriod, int size, int numberHalfPeriods, int lowCannyThreshold, int highCannyThreshold);


        void compute(cv::Mat image);

    };
#endif // USE_OPENCV
}

#endif