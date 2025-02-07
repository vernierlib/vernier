/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef TESTQRCODEDETECTOR_HPP
#define TESTQRCODEDETECTOR_HPP

#include "QRCodeDetector.hpp"
#include "UnitTest.hpp"

using namespace vernier;
using namespace std;
using namespace cv;

class TestQRCodeDetector {
public:
    
    static void main() {
        Mat image = imread("testfiles/QRCode/code23.png");

        QRCodeDetector detector;

        detector.fiducialDetector.lowCannyThreshold = 200;
        detector.fiducialDetector.highCannyThreshold = 400;

        detector.compute(image);
        detector.draw(image);
        cout << detector.toString() << endl;

        imshow("Edges", detector.fiducialDetector.cannyImage);
        imshow("Image", image);

        waitKey(0);
    }

    static void runAllTests() {
        QRCodeDetector detector;
        Mat image;

        image = imread("testfiles/QRCode/code11.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("testfiles/QRCode/code12.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("testfiles/QRCode/code13.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("testfiles/QRCode/code14.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("testfiles/QRCode/code15.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("testfiles/QRCode/code16.jpg");
        detector.fiducialDetector.lowCannyThreshold = 500;
        detector.fiducialDetector.highCannyThreshold = 600;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("testfiles/QRCode/code17.jpg");
        detector.fiducialDetector.lowCannyThreshold = 100;
        detector.fiducialDetector.highCannyThreshold = 200;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("testfiles/QRCode/code18.jpg");
        erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)), Point(-1, -1), 3);
        detector.fiducialDetector.lowCannyThreshold = 50;
        detector.fiducialDetector.highCannyThreshold = 100;
        UNIT_TEST(detector.codes.size() == 1);
        
        image = imread("testfiles/QRCode/code19.png");
        detector.fiducialDetector.lowCannyThreshold = 100;
        detector.fiducialDetector.highCannyThreshold = 200;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);        

        image = imread("testfiles/QRCode/code23.png");
        detector.fiducialDetector.lowCannyThreshold = 200;
        detector.fiducialDetector.highCannyThreshold = 400;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 2);        

        image = imread("testfiles/QRCode/code31.jpg");
        detector.fiducialDetector.lowCannyThreshold = 50;
        detector.fiducialDetector.highCannyThreshold = 100;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 3);

        image = imread("testfiles/QRCode/code61.jpg");
        detector.fiducialDetector.lowCannyThreshold = 120;
        detector.fiducialDetector.highCannyThreshold = 200;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 6);
    }

    static double speed(unsigned long testCount) {
        QRCodeDetector detector;
        Mat image;
        image = imread("testfiles/QRCode/code12.jpg");

        tic();
        for (unsigned long i = 0; i < testCount; i++) {
            detector.compute(image);
        }
        return toc(testCount);
    }
};


#endif