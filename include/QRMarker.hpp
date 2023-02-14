/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef QRMARKER_HPP
#define QRMARKER_HPP

#include "Common.hpp"

namespace Vernier {
#ifdef USE_OPENCV

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

    class QRMarker {
    public:

        cv::Point2d position;
        int patternCount;

        QRMarker(cv::Point2d position, int patternCount);

        std::string toString();

    };

#endif // USE_OPENCV
}

#endif