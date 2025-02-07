/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef TESTQRFIDUCIALDETECTOR_HPP
#define TESTQRFIDUCIALDETECTOR_HPP

#include "QRFiducialDetector.hpp"
#include "UnitTest.hpp"

using namespace vernier;
using namespace std;
using namespace cv;

class TestQRFiducialDetector {
public:

    static void main() {
#ifdef USE_OPENCV
        cv::Mat image = cv::imread("testfiles/QRCode/code17.jpg");

        QRFiducialDetector detector;

        detector.lowCannyThreshold = 150;
        detector.highCannyThreshold = 300;

        detector.compute(image);
        detector.drawRowPatterns(image);
        detector.drawColPatterns(image);
        detector.drawGroupsOfPatterns(image);
        detector.draw(image);
        cout << "Found " << detector.fiducials.size() << " markers." << std::endl;
        std::cout << detector.toString() << std::endl;

        cv::imshow("Edges", detector.cannyImage);
        cv::imshow("Image", image);
        cv::waitKey(0);
#endif //USE_OPENCV 
    }

    static void test(string filename, int lowCannyThreshold, int highCannyThreshold, int numberOfMarkers) {
#ifdef USE_OPENCV
        START_UNIT_TEST;
        cv::Mat image = imread(filename);

        QRFiducialDetector detector;

        detector.lowCannyThreshold = lowCannyThreshold;
        detector.highCannyThreshold = highCannyThreshold;

        detector.compute(image);
        cout << "Found " << detector.fiducials.size() << " markers in " << filename << std::endl;
        cout << detector.toString() << endl;

        UNIT_TEST(detector.fiducials.size() == numberOfMarkers);
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
        QRFiducialDetector detector;
        cv::Mat image;
        image = cv::imread("testfiles/QRCode/code12.jpg");
        detector.lowCannyThreshold = 50;
        detector.highCannyThreshold = 110;
        
        tic();
        for (unsigned long i = 0; i < testCount; i++) {
            detector.compute(image);
        }
#endif //USE_OPENCV
        return toc(testCount);
    }
};


#endif