/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef HPCODEPATTERNDETECTOR_HPP
#define HPCODEPATTERNDETECTOR_HPP

#include "PeriodicPatternDetector.hpp"
#include "QRCodeDetector.hpp"
#include "PatternPhase.hpp"
#include <map>

namespace vernier {

    class HPCodePatternDetector : public PeriodicPatternDetector {
    protected:

        Eigen::ArrayXXcd snapshot;
        int numberHalfPeriods;
        int snapshotSize;

        void readJSON(rapidjson::Value& document)  override;

        void takeSnapshot(int x, int y, cv::Mat image);

        unsigned long readNumber(QRCode& code, const cv::Mat& image, double dotSize);

    public:

        QRCodeDetector detector;
        //PatternPhase patternPhase;

        /** Map of detected codes (key map = code id) */
        std::map<int, Pose> codes;

        /** Constructs a pose estimator for periodic HP codes
         *
         *	\param physicalPeriod: physical period between the dots of the HP code
         *	\param snapshotSize: maximal size of the HP code in pixels
         *	\param numberHalfPeriods: number of half periods contained in the HP along one direction.
         */
        HPCodePatternDetector(double physicalPeriod = 1.0, int snapshotSize = 128, int numberHalfPeriods = 37);

        /** Prepares the different required objects for processing
         *
         *	\param physicalPeriod: physical period between the dots of the HP code
         *	\param snapshotSize: maximal size of the HP code in pixels
         *	\param numberHalfPeriods: number of half periods contained in the HP along one direction.
         */
        void resize(double physicalPeriod, int snapshotSize, int numberHalfPeriods);

        /** Estimate the pose of all HP codes in an image */
        void compute(const cv::Mat& image) override;

        Pose get2DPose(int id) override;

        Pose get3DPose(int id) override;

        std::vector<Pose> getAll3DPoses(int id) override;

        bool patternFound(int id) override;

        /** Draws all the found patterns in a image (analysis must have been done before) */
        void draw(cv::Mat& image);

        void showControlImages(int delay = -1) override;

    };

}

#endif