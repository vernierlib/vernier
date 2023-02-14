/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef QRMARKERDETECTOR_HPP
#define QRMARKERDETECTOR_HPP

#include "Common.hpp"
#include "QRMarker.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    class QRMarkerDetector {
    private:
        std::vector<QRRowPattern> rowPatterns;
        std::vector<QRColumnPattern> colPatterns;

        std::vector<int> colPatternGroup;
        std::vector<int> rowPatternGroup;

        std::vector<std::vector<int> > groupsOfColPatterns;
        std::vector<std::vector<int> > groupsOfRowPatterns;

        void findRowPatterns();
        void findColPatterns();
        void clearGroups();
        void findGroupsOfPatterns();
        void sortMarkers();

    public:

        /** Canny image */
        cv::Mat cannyImage;

        /** Grayscale image */
        cv::Mat grayImage;

        /** Low threshold for the hysteresis procedure */
        int lowCannyThreshold = 100;

        /** High threshold for the hysteresis procedure */
        int highCannyThreshold = 200;

        /** Relative error threshold for patterns detection */
        int proportionToleranceInPixels = 3;

        /** List of detected markers in descending order of pattern count */
        std::vector<QRMarker> markers;

        /** Constructs a detector for QR markers */
        QRMarkerDetector();

        /** Detects QR markers in an image */
        void compute(cv::Mat& image);

        /** Draws the found row patterns in a image (detection must have been done before)
         *
         *	\param image: image of any types
         *  \param color: color of drawings
         */
        void drawRowPatterns(cv::Mat& image, cv::Scalar color = cv::Scalar(0, 0, 255));

        /** Draws the found column patterns in a image (detection must have been done before)
         *
         *	\param image: image of any types
         *  \param color: color of drawings
         */
        void drawColPatterns(cv::Mat& image, cv::Scalar color = cv::Scalar(0, 0, 255));

        /** Draws the found groups of patterns (detection must have been done before)
         *
         *	\param image: image of any types
         *  \param color: color of drawings
         */
        void drawGroupsOfPatterns(cv::Mat& image, cv::Scalar color = cv::Scalar(0, 255, 0));

        /** Draws the found markers (detection must have been done before)
         *
         *	\param image: image of any types
         *  \param color: color of drawings
         */
        void drawMarkers(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 0, 0));

        std::string toString();
    };

#endif // USE_OPENCV
}

#endif