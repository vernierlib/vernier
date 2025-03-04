/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "SquareDetector.hpp"


// Mandatory includes to avoid error: 
//   member access into incomplete type 'cv::aruco::ArucoDetector::ArucoDetectorImpl'
//   note: forward declaration of 'cv::aruco::ArucoDetector::ArucoDetectorImpl'
#include "objdetect/src/aruco/aruco_detector.cpp"
#include "objdetect/src/aruco/aruco_dictionary.cpp"
#include "objdetect/src/aruco/aruco_board.cpp"
#include "objdetect/src/aruco/aruco_utils.cpp"
#include "objdetect/src/aruco/apriltag/apriltag_quad_thresh.cpp"
#include "objdetect/src/aruco/apriltag/zmaxheap.cpp"


namespace vernier {

    Square::Square(cv::Point2f corner0, cv::Point2f corner1, cv::Point2f corner2, cv::Point2f corner3) {
        topLeft = corner0;
        topRight = corner1;
        bottomRight = corner2;
        bottomLeft = corner3;
    }

    Square::Square(std::vector<cv::Point2f> corners) {
        ASSERT(corners.size() == 4);
        topLeft = corners[0];
        topRight = corners[1];
        bottomRight = corners[2];
        bottomLeft = corners[3];
    }

    cv::Point2f Square::getCenter() {
        return (topRight + topLeft + bottomRight + bottomLeft) / 4.0;
    }

    double Square::getAngle() {
        cv::Point2f direction = ((topRight - topLeft) + (bottomRight - bottomLeft)) / 2.0;
        return atan2(direction.y, direction.x);
    }

    void Square::draw(cv::Mat& image, cv::Scalar color) {
        cv::line(image, topLeft, topRight, color, 2);
        cv::line(image, topLeft, bottomLeft, color, 2);
        cv::line(image, topRight, bottomRight, color, 2);
        cv::line(image, bottomLeft, bottomRight, color, 2);
    }

    double Square::getRadius() {
        double diagonal1 = cv::norm(topLeft - bottomRight);
        double diagonal2 = cv::norm(topRight - bottomLeft);
        return std::max(diagonal1, diagonal2) / 2.0;
    }

    std::string Square::toString() {
        return "[ topLeft=(" + vernier::to_string(topLeft.x) + "," + vernier::to_string(topLeft.y)
                + "), topRight=(" + vernier::to_string(topRight.x) + "," + vernier::to_string(topRight.y)
                + "), bottomRight=(" + vernier::to_string(bottomRight.x) + "," + vernier::to_string(bottomRight.y)
                + "), bottomLeft=(" + vernier::to_string(bottomLeft.x) + "," + vernier::to_string(bottomLeft.y) + " ]";
    }

    SquareDetector::SquareDetector() {
        parameters = cv::aruco::DetectorParameters();
    }

    void SquareDetector::compute(const cv::Mat& image) {

        cv::aruco::ArucoDetector detector(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250), parameters);

        std::vector<std::vector<cv::Point> > contours;
        std::vector<std::vector<cv::Point2f> > candidates;
        detector.arucoDetectorImpl->detectCandidates(image, candidates, contours);

        std::vector<cv::aruco::MarkerCandidateTree> selectedCandidates;
        selectedCandidates = detector.arucoDetectorImpl->filterTooCloseCandidates(candidates, contours);

        squares.clear();
        for (int i = 0; i < selectedCandidates.size(); i++) {
            if (selectedCandidates[i].parent < 0) {
                Square s(selectedCandidates[i].corners);
                squares.push_back(s);
            }
        }
    }

    void SquareDetector::draw(cv::Mat& image) {
        for (int i = 0; i < squares.size(); i++) {
            squares[i].draw(image);
        }
    }

    std::string SquareDetector::toString() {
        std::string result = "{ ";
        for (int i = 0; i < squares.size(); i++) {
            result += squares[i].toString() + " ; ";
        }
        result += " }";
        return result;
    }

}