/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#ifndef SQUAREDETECTOR_HPP
#define SQUAREDETECTOR_HPP

#include "Common.hpp"
#include "objdetect/include/objdetect/aruco_detector.hpp"

//class CustomArucoDetector : protected cv::aruco::ArucoDetector {
//    public:
//        using cv::aruco::ArucoDetector::ArucoDetector; // Hérite du constructeur
//
//        // Méthode pour exposer detectCandidates()
//
//        void CustomArucoDetector::detectCandidates2(const cv::Mat& image, std::vector<std::vector<cv::Point2f>>&candidates, std::vector<std::vector<cv::Point>>&contours) {
//            arucoDetectorImpl->detectCandidates(image, candidates, contours);
//        }
//    };

namespace vernier {
#ifdef USE_OPENCV

    class Square {
    public:

        cv::Point2f topRight, topLeft, bottomRight, bottomLeft;

        /** Constructs a default Square */
        Square() {
        };

        /** Constructs a Square with four points */
        Square(cv::Point2f corner0, cv::Point2f corner1, cv::Point2f corner2, cv::Point2f corner3);

        /** Constructs a Square with a vector of four points */
        Square(std::vector<cv::Point2f> corners);

        /** Draws the Square */
        void draw(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 0, 0));

        /** Returns the half length of the diagonal of the Square */
        double getRadius();

        /** Returns the angle of the Square from x axis */
        double getAngle();

        /** Returns the center of the Square */
        cv::Point2f getCenter();

        /** Converts into a string */
        std::string toString();
    };

    class SquareDetector {
    public:
        
        /** Parameters of the square detector (see OpenCV documention about ArUco detector) */
        cv::aruco::DetectorParameters parameters;

        /** Vector of detected squares (unsort) */
        std::vector<Square> squares;

        /** Constructs a square detector */
        SquareDetector();

        /** Detects squares in an image (type should be CV_8UC1) */
        void compute(const cv::Mat& image);

        /** Draws the found squares in an image (detection must have been done before) */
        void draw(cv::Mat& image);

        /** Converts into a string */
        std::string toString();
    };
#endif // USE_OPENCV
}

#endif