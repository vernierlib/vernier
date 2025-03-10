/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#ifndef STAMPPATTERNDETECTOR_HPP
#define STAMPPATTERNDETECTOR_HPP

#include "PeriodicPatternDetector.hpp"
#include "SquareDetector.hpp"
#include "PatternPhase.hpp"
#include <map>

namespace vernier {

    class StampPatternDetector : public PeriodicPatternDetector {
    protected:

        Eigen::ArrayXXcd snapshot;
        int numberHalfPeriods;
        int snapshotSize;
        
        void readJSON(rapidjson::Value& document) override;

        void takeSnapshot(int x, int y, cv::Mat image);

    public:

        SquareDetector detector;
        
        /** Vector of detected stamps */
        std::vector<Pose> stamps;

        /** Constructs a pose estimator for stamps
         *
         *	\param physicalPeriod: physical period between the dots of the stamp
         *	\param snapshotSize: maximal size of the stamp in pixels
         *	\param numberHalfPeriods: number of half periods contained in the stamp along one direction.
         */
        StampPatternDetector(double physicalPeriod = 1.0, int snapshotSize = 128, int numberHalfPeriods = 61);

        /** Prepares the different required objects for processing
         *
         *	\param physicalPeriod: physical period between the dots of the stamp
         *	\param snapshotSize: maximal size of the stamp in pixels
         *	\param numberHalfPeriods: number of half periods contained in the stamp one direction.
         */
        void resize(double physicalPeriod, int snapshotSize, int numberHalfPeriods);

        /** Estimate the pose of all stamps in an image */
        void compute(const cv::Mat& image) override;

        Pose get2DPose(int id) override;

        Pose get3DPose(int id) override;

        std::vector<Pose> getAll3DPoses(int id) override;

        bool patternFound(int id) override;

        /** Draws all the found stamps in a image (analysis must have been done before) */
        void draw(cv::Mat& image);

        void showControlImages(int delay = -1) override;

    };

}

#endif