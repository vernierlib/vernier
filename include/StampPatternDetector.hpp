/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#ifndef STAMPPATTERNDETECTOR_HPP
#define STAMPPATTERNDETECTOR_HPP

#include "SquareDetector.hpp"
#include "PatternPhase.hpp"
#include <map>

namespace vernier {

    class StampPatternDetector {
    protected:

        Eigen::ArrayXXcd snapshot;
        int numberHalfPeriods;
        double physicalPeriod;
        int snapshotSize;

        void takeSnapshot(int x, int y, cv::Mat image);

    public:

        SquareDetector detector;
        PatternPhase patternPhase;

        /** Vector of detected stamps */
        std::vector<Pose> stamps;

        /** Constructs a pose estimator for stamps
         *
         *	\param physicalPeriod: physical period between the dots of the QR code
         *	\param snapshotSize: maximal size of the QR code in pixels
         *	\param numberHalfPeriods: number of half periods contained in the QR along one direction.
         */
        StampPatternDetector(double physicalPeriod = 1.0, int snapshotSize = 128, int numberHalfPeriods = 61);

        /** Prepares the different required objects for processing
         *
         *	\param physicalPeriod: physical period between the dots of the QR code
         *	\param snapshotSize: maximal size of the QR code in pixels
         *	\param numberHalfPeriods: number of half periods contained in the QR along one direction.
         */
        void resize(double physicalPeriod, int snapshotSize, int numberHalfPeriods);

        /** Estimate the pose of a stamp in a image
         *
         *	\param image: image
         */
        void compute(const cv::Mat& image);


        /** Draws all the found stamps in a image (analysis must have been done before)
         *
         *	\param image: image of any types
         */
        void draw(cv::Mat& image);

        void showControlImages();

    };

}

#endif