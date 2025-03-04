/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef QRFIDUCIALDETECTOR_HPP
#define QRFIDUCIALDETECTOR_HPP

#include "Common.hpp"

namespace vernier {

    class QRColumnPattern {
    private:
        int yA, yB, yC, yD, yE, yF;

    public:
        int col;
        QRColumnPattern(int col);
        void pushRow(int row);
        bool isWellProportioned(int proportionToleranceInPixels);
        double getRow();
        double getDotSize();
        void draw(cv::Mat& image, cv::Scalar color);
        friend class QRRowPattern;
    };

    class QRRowPattern {
    private:
        int xA, xB, xC, xD, xE, xF;
    public:
        int row;
        QRRowPattern(int row);
        void pushCol(int col);
        bool isWellProportioned(int proportionToleranceInPixels);
        double getCol();
        double getDotSize();
        void draw(cv::Mat& image, cv::Scalar color);
        bool isCrossing(QRColumnPattern& colPattern);
    };

    class QRFiducialPattern {
    public:
        cv::Point2d position;
        int patternCount;
        QRFiducialPattern(cv::Point2d position, int patternCount);
        std::string toString();
    };

    class QRFiducialDetector {
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
        void sortFiducials();

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

        /** List of detected position patterns in descending order of pattern count */
        std::vector<QRFiducialPattern> fiducials;

        /** Constructs a detector for QR markers */
        QRFiducialDetector();

        /** Detects QR markers in an image */
        void compute(cv::Mat& image);

        /** Draws the found row patterns in a image (detection must have been done before)
         *
         *  \param image: image of any types
         *  \param color: color of drawings
         */
        void drawRowPatterns(cv::Mat& image, cv::Scalar color = cv::Scalar(0, 0, 255));

        /** Draws the found column patterns in a image (detection must have been done before)
         *
         *  \param image: image of any types
         *  \param color: color of drawings
         */
        void drawColPatterns(cv::Mat& image, cv::Scalar color = cv::Scalar(0, 0, 255));

        /** Draws the found groups of patterns (detection must have been done before)
         *
         * \param image: image of any types
         * \param color: color of drawings
         */
        void drawGroupsOfPatterns(cv::Mat& image, cv::Scalar color = cv::Scalar(0, 255, 0));

        /** Draws the found fiducials (detection must have been done before)
         *
         *  \param image: image of any types
         *  \param color: color of drawings
         */
        void draw(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 0, 0));

        std::string toString();
    };

}

#endif