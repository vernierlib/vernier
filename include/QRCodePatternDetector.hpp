/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef QRCODEPATTERNDETECTOR_HPP
#define QRCODEPATTERNDETECTOR_HPP

#include "BitmapPatternDetector.hpp"

namespace vernier {

    class QRCodePatternDetector : public BitmapPatternDetector {
    protected:
        Eigen::ArrayXXd finderPattern;
        int qrCodeSize;

        void createFinderPattern();

        void readJSON(rapidjson::Value& document);

    public:
        QRCodePatternDetector();

        QRCodePatternDetector(int qrCodeSize, double approxPixelPeriod, double physicalPeriod = 0);

        void resize(int qrCodeSize, double approxPixelPeriod, double physicalPeriod = 0);

        void compute(Eigen::ArrayXXd& pattern);

#ifdef USE_OPENCV
        void computePerspective(Eigen::ArrayXXd& pattern);
#endif // USE_OPENCV

        Pose getCameraPoseInPattern();

        Pose getPatternPoseInCamera();

        std::vector<Pose> getAll3DPoses();
    };
}

#endif