/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "QRMarkerDetector.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    void QRMarkerDetector::findRowPatterns() {
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

    void QRMarkerDetector::findColPatterns() {
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

    void QRMarkerDetector::clearGroups() {
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

    void QRMarkerDetector::findGroupsOfPatterns() {
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

    void QRMarkerDetector::sortMarkers() {
        markers.clear();
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
                markers.push_back(QRMarker(cv::Point2d(x, y), patternCount));
                int i = markers.size() - 1;
                while (i > 0 && markers[i - 1].patternCount < markers[i].patternCount) {
                    std::swap(markers[i], markers[i - 1]);
                    i--;
                }
            }
        }
    }

    QRMarkerDetector::QRMarkerDetector() {
        rowPatterns.reserve(512);
        colPatterns.reserve(512);
        colPatternGroup.reserve(128);
        rowPatternGroup.reserve(128);
    }

    void QRMarkerDetector::compute(cv::Mat& image) {
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
        sortMarkers();
    }

    void QRMarkerDetector::drawRowPatterns(cv::Mat& image, cv::Scalar color) {
        for (int r = 0; r < rowPatterns.size(); r++) {
            rowPatterns[r].draw(image, color);
        }
    }

    void QRMarkerDetector::drawColPatterns(cv::Mat& image, cv::Scalar color) {
        for (int c = 0; c < colPatterns.size(); c++) {
            colPatterns[c].draw(image, color);
        }
    }

    void QRMarkerDetector::drawGroupsOfPatterns(cv::Mat& image, cv::Scalar color) {
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

    void QRMarkerDetector::drawMarkers(cv::Mat& image, cv::Scalar color) {
        for (int i = 0; i < markers.size(); i++) {
            cv::circle(image, markers[i].position, 5, color, -1);
        }
    }

    std::string QRMarkerDetector::toString() {
        std::string result = "{ ";
        for (int i = 0; i < markers.size(); i++) {
            result += markers[i].toString() + " ; ";
        }
        result += " }";
        return result;
    }
#endif //USE_OPENCV
}