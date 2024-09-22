/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTQRMARKERDETECTOR_HPP
#define TESTQRMARKERDETECTOR_HPP

#include "QRMarkerDetector.hpp"
#include "UnitTest.hpp"

using namespace Vernier;
using namespace std;
using namespace cv;

class TestQRMarkerDetector {
public:

    static void main() {
#ifdef USE_OPENCV
        cv::Mat image = cv::imread("testfiles/QRCode/code17.jpg");

        QRMarkerDetector markerDetector;

        markerDetector.lowCannyThreshold = 150;
        markerDetector.highCannyThreshold = 300;

        markerDetector.compute(image);
        markerDetector.drawRowPatterns(image);
        markerDetector.drawColPatterns(image);
        markerDetector.drawGroupsOfPatterns(image);
        markerDetector.drawMarkers(image);
        cout << "Found " << markerDetector.markers.size() << " markers." << std::endl;
        std::cout << markerDetector.toString() << std::endl;

        cv::imshow("Edges", markerDetector.cannyImage);
        cv::imshow("Image", image);
        cv::waitKey(0);
#endif //USE_OPENCV 
    }

    static void test(string filename, int lowCannyThreshold, int highCannyThreshold, int numberOfMarkers) {
#ifdef USE_OPENCV
        START_UNIT_TEST;
        cv::Mat image = imread(filename);

        QRMarkerDetector markerDetector;

        markerDetector.lowCannyThreshold = lowCannyThreshold;
        markerDetector.highCannyThreshold = highCannyThreshold;

        markerDetector.compute(image);
        cout << "Found " << markerDetector.markers.size() << " markers in " << filename << std::endl;
        cout << markerDetector.toString() << endl;

        UNIT_TEST(markerDetector.markers.size() == numberOfMarkers);
#endif //USE_OPENCV            
    }

    static void runAllTests() {
        test("testfiles/QRCode/code11.jpg", 200, 400, 3);
        test("testfiles/QRCode/code12.jpg", 200, 400, 3);
        test("testfiles/QRCode/code13.jpg", 200, 400, 3);
        test("testfiles/QRCode/code14.jpg", 200, 400, 3);
        test("testfiles/QRCode/code15.jpg", 200, 400, 3);
        test("testfiles/QRCode/code16.jpg", 350, 700, 3);
        test("testfiles/QRCode/code17.jpg", 100, 200, 3);
        test("testfiles/QRCode/code21.jpg", 200, 550, 6);
        test("testfiles/QRCode/code22.tif", 50, 100, 6);
        test("testfiles/QRCode/code23.png", 200, 400, 6);
        test("testfiles/QRCode/code31.jpg", 50, 110, 9);
        test("testfiles/QRCode/code61.jpg", 100, 210, 18);
    }

    static double speed(unsigned long testCount) {
#ifdef USE_OPENCV
        QRMarkerDetector markerDetector;
        cv::Mat image;
        image = cv::imread("testfiles/QRCode/code12.jpg");
        markerDetector.lowCannyThreshold = 50;
        markerDetector.highCannyThreshold = 110;
        
        tic();
        for (unsigned long i = 0; i < testCount; i++) {
            markerDetector.compute(image);
        }
#endif //USE_OPENCV
        return toc(testCount);
    }
};


#endif