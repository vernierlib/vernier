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

        Eigen::ArrayXXd window;
        Eigen::ArrayXXd snapshot;
        int numberHalfPeriods;
        int snapshotSize;

        void readJSON(const rapidjson::Value& document)  override;
        
        void computeImage() override;

        unsigned long readNumber(QRCode& code, const cv::Mat& image, double dotSize);

    public:

        QRCodeDetector detector;
        
        /** Map of detected codes (key map = code id) */
        std::map<int, Pose> markers;

        /** Constructs a pose estimator for periodic HP codes
         *
         *	\param physicalPeriod: physical period between the dots of the HP code
         *	\param numberHalfPeriods: number of half periods contained in the HP along one direction.
         *	\param snapshotSize: maximal size of the HP code in pixels
         */
        HPCodePatternDetector(double physicalPeriod = 1.0, int numberHalfPeriods = 37, int snapshotSize = 128);

        /** Prepares the different required objects for processing
         *
         *	\param physicalPeriod: physical period between the dots of the HP code
         *	\param numberHalfPeriods: number of half periods contained in the HP along one direction.
         *	\param snapshotSize: maximal size of the HP code in pixels
         */
        void resize(double physicalPeriod, int numberHalfPeriods, int snapshotSize);

        Pose get2DPose(int id = -1) override;

        Pose get3DPose(int id = -1) override;

        std::vector<Pose> getAll3DPoses(int id = -1) override;

        bool patternFound(int id = -1) override;
        
        int patternCount() override;

        void draw(cv::Mat& image) override;

        void showControlImages() override;

    };

}

#endif