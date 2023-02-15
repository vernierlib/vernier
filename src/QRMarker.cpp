/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "QRMarker.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    QRColumnPattern::QRColumnPattern(int col) : col(col) {
    };

    void QRColumnPattern::pushRow(int row) {
        yA = yB;
        yB = yC;
        yC = yD;
        yD = yE;
        yE = yF;
        yF = row;
    }

    //    bool QRColumnPattern::isWellProportioned(int proportionToleranceInPixels) {
    //        int dAE = yE - yA;
    //        int dBF = yF - yB;
    //        int dAC = yC - yA;
    //        int dDF = yF - yD;
    //        int dBD = yD - yB;
    //        int dCE = yE - yC;
    //        return (dAE > dBF - proportionToleranceInPixels && dAE < dBF + proportionToleranceInPixels &&
    //                6 * dAC > 2 * dAE - 2 * proportionToleranceInPixels && 5 * dAC < 2 * dAE + 2 * proportionToleranceInPixels &&
    //                6 * dDF > 2 * dBF - 2 * proportionToleranceInPixels && 5 * dDF < 2 * dBF + 2 * proportionToleranceInPixels &&
    //                5 * dBD > 3 * dBF - 3 * proportionToleranceInPixels && 6 * dBD < 4 * dBF + 4 * proportionToleranceInPixels &&
    //                5 * dCE > 3 * dAE - 3 * proportionToleranceInPixels && 6 * dCE < 4 * dAE + 4 * proportionToleranceInPixels);
    //    }

    bool QRColumnPattern::isWellProportioned(int proportionToleranceInPixels) {
        int dAB = yB - yA;
        int dBC = yC - yB;
        int dCD = yD - yC;
        int dDE = yE - yD;
        int dEF = yF - yE;
        int dAF = yF - yA;
        return (dAB > dEF - proportionToleranceInPixels && dAB < dEF + proportionToleranceInPixels &&
                dBC > dDE - proportionToleranceInPixels && dBC < dDE + proportionToleranceInPixels &&
                3 * dAF > 7 * dCD - 3 * proportionToleranceInPixels && 2 * dAF < 7 * dCD + 3 * proportionToleranceInPixels);
    }

    double QRColumnPattern::getRow() {
        return (double) (yA + yB + yC + yD + yE + yF) / 6.0;
    }

    double QRColumnPattern::getDotSize() {
        int dAE = yE - yA;
        int dBF = yF - yB;
        return (double) (dAE + dBF) / 12.0;
    }

    void QRColumnPattern::draw(cv::Mat& image, cv::Scalar color) {
//        drawPixel(image, col, yA, color);
//        drawPixel(image, col, yB, color);
//        drawPixel(image, col, yC, color);
//        drawPixel(image, col, yD, color);
//        drawPixel(image, col, yE, color);
//        drawPixel(image, col, yF, color);
    }

    QRRowPattern::QRRowPattern(int row) : row(row) {
    };

    void QRRowPattern::pushCol(int col) {
        xA = xB;
        xB = xC;
        xC = xD;
        xD = xE;
        xE = xF;
        xF = col;
    }

    //    bool QRRowPattern::isWellProportioned(int proportionToleranceInPixels) {
    //        int dAE = xE - xA;
    //        int dBF = xF - xB;
    //        int dAC = xC - xA;
    //        int dDF = xF - xD;
    //        int dBD = xD - xB;
    //        int dCE = xE - xC;
    //        return (dAE > dBF - proportionToleranceInPixels && dAE < dBF + proportionToleranceInPixels &&
    //                6 * dAC > 2 * dAE - 2 * proportionToleranceInPixels && 5 * dAC < 2 * dAE + 2 * proportionToleranceInPixels &&
    //                6 * dDF > 2 * dBF - 2 * proportionToleranceInPixels && 5 * dDF < 2 * dBF + 2 * proportionToleranceInPixels &&
    //                5 * dBD > 3 * dBF - 3 * proportionToleranceInPixels && 6 * dBD < 4 * dBF + 4 * proportionToleranceInPixels &&
    //                5 * dCE > 3 * dAE - 3 * proportionToleranceInPixels && 6 * dCE < 4 * dAE + 4 * proportionToleranceInPixels);
    //    }

    bool QRRowPattern::isWellProportioned(int proportionToleranceInPixels) {
        int dAB = xB - xA;
        int dBC = xC - xB;
        int dCD = xD - xC;
        int dDE = xE - xD;
        int dEF = xF - xE;
        int dAF = xF - xA;
        return (dAB > dEF - proportionToleranceInPixels && dAB < dEF + proportionToleranceInPixels &&
                dBC > dDE - proportionToleranceInPixels && dBC < dDE + proportionToleranceInPixels &&
                3 * dAF > 7 * dCD - 3 * proportionToleranceInPixels && 2 * dAF < 7 * dCD + 3 * proportionToleranceInPixels);
    }

    double QRRowPattern::getCol() {
        return (double) (xA + xB + xC + xD + xE + xF) / 6.0;
    }

    double QRRowPattern::getDotSize() {
        int dAE = xE - xA;
        int dBF = xF - xB;
        return (double) (dAE + dBF) / 12.0;
    }

    void QRRowPattern::draw(cv::Mat& image, cv::Scalar color) {
//        drawPixel(image, xA, row, color);
//        drawPixel(image, xB, row, color);
//        drawPixel(image, xC, row, color);
//        drawPixel(image, xD, row, color);
//        drawPixel(image, xE, row, color);
//        drawPixel(image, xF, row, color);
    }

    bool QRRowPattern::isCrossing(QRColumnPattern& colPattern) {
        return (colPattern.col > xC && colPattern.col < xD && row > colPattern.yC && row < colPattern.yD);
    }

    QRMarker::QRMarker(cv::Point2d position, int patternCount) :
    position(position), patternCount(patternCount) {
    }

    std::string QRMarker::toString() {
        return "[(" + Vernier::toString(position.x) + "," + Vernier::toString(position.y) + "), " + Vernier::toString(patternCount) + "]";
    }
#endif //USE_OPENCV
}