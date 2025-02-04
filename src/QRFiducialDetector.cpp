/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "QRFiducialDetector.hpp"

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

    QRFiducialPattern::QRFiducialPattern(cv::Point2d position, int patternCount) :
    position(position), patternCount(patternCount) {
    }

    std::string QRFiducialPattern::toString() {
        return "[(" + Vernier::toString(position.x) + "," + Vernier::toString(position.y) + "), " + Vernier::toString(patternCount) + "]";
    }

    void QRFiducialDetector::findRowPatterns() {
        rowPatterns.clear();
        int pixelIndex = 0;
        for (int y = 0; y < cannyImage.rows; y++) {
            int egdeCount = 0;
            int insideEdge = false;
            int edgeStart;
            QRRowPattern rowPattern(y);
            for (int x = 0; x < cannyImage.cols - 1; x++) {
                if (cannyImage.data[pixelIndex] > 0 && !insideEdge) { // entering an edge
                    insideEdge = true;
                    edgeStart = x;
                } else if (cannyImage.data[pixelIndex] == 0 && cannyImage.data[pixelIndex + 1] == 0 && insideEdge) { // exiting an edge
                    insideEdge = false;
                    rowPattern.pushCol(((x - 1) + edgeStart) / 2);
                    egdeCount++;
                    if (egdeCount >= 6 && rowPattern.isWellProportioned(proportionToleranceInPixels)) {
                        rowPatterns.push_back(rowPattern);
                    }
                }
                pixelIndex++;
            }
            pixelIndex++;
        }
    }

    void QRFiducialDetector::findColPatterns() {
        colPatterns.clear();
        for (int x = 0; x < cannyImage.cols; x++) {
            int egdeCount = 0;
            int pixelIndex = x;
            bool insideEdge = false;
            int edgeStart;
            QRColumnPattern colPattern(x);
            for (int y = 0; y < cannyImage.rows - 1; y++) {
                if (cannyImage.data[pixelIndex] > 0 && !insideEdge) { // entering an edge
                    insideEdge = true;
                    edgeStart = y;
                } else if (cannyImage.data[pixelIndex] == 0 && cannyImage.data[pixelIndex + cannyImage.cols] == 0 && insideEdge) { // exiting an edge
                    insideEdge = false;
                    colPattern.pushRow(((y - 1) + edgeStart) / 2);
                    egdeCount++;
                    if (egdeCount >= 6 && colPattern.isWellProportioned(proportionToleranceInPixels)) {
                        colPatterns.push_back(colPattern);
                    }
                }
                pixelIndex += cannyImage.cols;
            }
        }
    }

    void QRFiducialDetector::clearGroups() {
        colPatternGroup.resize(colPatterns.size());
        for (int i = 0; i < colPatternGroup.size(); i++) {
            colPatternGroup[i] = -1;
        }
        rowPatternGroup.resize(rowPatterns.size());
        for (int i = 0; i < rowPatternGroup.size(); i++) {
            rowPatternGroup[i] = -1;
        }
        groupsOfColPatterns.clear();
        groupsOfRowPatterns.clear();
    }

    void QRFiducialDetector::findGroupsOfPatterns() {
        clearGroups();
        for (int row = 0; row < rowPatterns.size(); row++) {
            if (rowPatternGroup[row] < 0) { // row pattern not in a group
                for (int col = 0; col < colPatterns.size(); col++) {
                    if (colPatternGroup[col] < 0) { // col pattern not in a group
                        if (rowPatterns[row].isCrossing(colPatterns[col])) { // found a new group
                            int groupNumber = groupsOfColPatterns.size();
                            rowPatternGroup[row] = groupNumber;
                            colPatternGroup[col] = groupNumber;
                            groupsOfColPatterns.push_back(std::vector<int>());
                            groupsOfRowPatterns.push_back(std::vector<int>());
                            groupsOfColPatterns[groupNumber].push_back(col);
                            groupsOfRowPatterns[groupNumber].push_back(row);

                            // look for other crossing patterns
                            for (int c = 0; c < colPatterns.size(); c++) {
                                if (colPatternGroup[c] < 0 && rowPatterns[row].isCrossing(colPatterns[c])) { // extending an existing group from row
                                    colPatternGroup[c] = groupNumber;
                                    groupsOfColPatterns[groupNumber].push_back(c);
                                }
                            }

                            for (int r = 0; r < rowPatterns.size(); r++) {
                                if (rowPatternGroup[r] < 0 && rowPatterns[r].isCrossing(colPatterns[col])) { // extending an existing group from col
                                    rowPatternGroup[r] = groupNumber;
                                    groupsOfRowPatterns[groupNumber].push_back(r);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void QRFiducialDetector::sortFiducials() {
        fiducials.clear();
        for (int groupNumber = 0; groupNumber < groupsOfRowPatterns.size(); groupNumber++) {
            double dotSize = 0.0;

            // subpixel column calculation
            double x = 0.0;
            for (int i = 0; i < groupsOfRowPatterns[groupNumber].size(); i++) {
                int r = groupsOfRowPatterns[groupNumber][i];
                x += rowPatterns[r].getCol();
                dotSize += rowPatterns[r].getDotSize();
            }
            x /= groupsOfRowPatterns[groupNumber].size();

            // subpixel row calculation
            double y = 0.0;
            for (int i = 0; i < groupsOfColPatterns[groupNumber].size(); i++) {
                int c = groupsOfColPatterns[groupNumber][i];
                y += colPatterns[c].getRow();
                dotSize += colPatterns[c].getDotSize();
            }
            y /= groupsOfColPatterns[groupNumber].size();

            int patternCount = (groupsOfColPatterns[groupNumber].size() + groupsOfRowPatterns[groupNumber].size());
            dotSize /= patternCount;
            if (patternCount >= 2 * dotSize) {
                fiducials.push_back(QRFiducialPattern(cv::Point2d(x, y), patternCount));
                int i = fiducials.size() - 1;
                while (i > 0 && fiducials[i - 1].patternCount < fiducials[i].patternCount) {
                    std::swap(fiducials[i], fiducials[i - 1]);
                    i--;
                }
            }
        }
    }

    QRFiducialDetector::QRFiducialDetector() {
        rowPatterns.reserve(512);
        colPatterns.reserve(512);
        colPatternGroup.reserve(128);
        rowPatternGroup.reserve(128);
    }

    void QRFiducialDetector::compute(cv::Mat& image) {
        if (image.channels() > 1) {
            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = image;
        }

        cv::normalize(image, image, 255, 0, cv::NORM_MINMAX);
        image.convertTo(grayImage, CV_8UC1);

        //cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        cv::Canny(grayImage, cannyImage, lowCannyThreshold, highCannyThreshold, 3, true);
        findRowPatterns();
        findColPatterns();
        findGroupsOfPatterns();
        sortFiducials();
    }

    void QRFiducialDetector::drawRowPatterns(cv::Mat& image, cv::Scalar color) {
        for (int r = 0; r < rowPatterns.size(); r++) {
            rowPatterns[r].draw(image, color);
        }
    }

    void QRFiducialDetector::drawColPatterns(cv::Mat& image, cv::Scalar color) {
        for (int c = 0; c < colPatterns.size(); c++) {
            colPatterns[c].draw(image, color);
        }
    }

    void QRFiducialDetector::drawGroupsOfPatterns(cv::Mat& image, cv::Scalar color) {
        for (int c = 0; c < colPatterns.size(); c++) {
            if (colPatternGroup[c] >= 0) {
                colPatterns[c].draw(image, color);
            }
        }
        for (int r = 0; r < rowPatterns.size(); r++) {
            if (rowPatternGroup[r] >= 0) {
                rowPatterns[r].draw(image, color);
            }
        }
    }

    void QRFiducialDetector::draw(cv::Mat& image, cv::Scalar color) {
        for (int i = 0; i < fiducials.size(); i++) {
            cv::circle(image, fiducials[i].position, 5, color, -1);
        }
    }

    std::string QRFiducialDetector::toString() {
        std::string result = "{ ";
        for (int i = 0; i < fiducials.size(); i++) {
            result += fiducials[i].toString() + " ; ";
        }
        result += " }";
        return result;
    }

#endif //USE_OPENCV
}