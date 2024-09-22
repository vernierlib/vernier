/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTQRCODEDETECTOR_HPP
#define TESTQRCODEDETECTOR_HPP

#include "QRCodeDetector.hpp"
#include "UnitTest.hpp"

using namespace Vernier;
using namespace std;
using namespace cv;

class TestQRCodeDetector {
public:
    
    static void main() {
        Mat image = imread("testfiles/QRCode/code23.png");

        QRCodeDetector codeDetector;

        codeDetector.markerDetector.lowCannyThreshold = 200;
        codeDetector.markerDetector.highCannyThreshold = 400;

        codeDetector.compute(image);
        codeDetector.draw(image);
        cout << codeDetector.toString() << endl;

        imshow("Edges", codeDetector.markerDetector.cannyImage);
        imshow("Image", image);

        waitKey(0);
    }

    static void runAllTests() {
        QRCodeDetector codeDetector;
        Mat image;

        image = imread("testfiles/QRCode/code11.jpg");
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 1);

        image = imread("testfiles/QRCode/code12.jpg");
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 1);

        image = imread("testfiles/QRCode/code13.jpg");
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 1);

        image = imread("testfiles/QRCode/code14.jpg");
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 1);

        image = imread("testfiles/QRCode/code15.jpg");
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 1);

        image = imread("testfiles/QRCode/code16.jpg");
        codeDetector.markerDetector.lowCannyThreshold = 500;
        codeDetector.markerDetector.highCannyThreshold = 600;
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 1);

        image = imread("testfiles/QRCode/code17.jpg");
        codeDetector.markerDetector.lowCannyThreshold = 100;
        codeDetector.markerDetector.highCannyThreshold = 200;
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 1);

        image = imread("testfiles/QRCode/code18.jpg");
        erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)), Point(-1, -1), 3);
        codeDetector.markerDetector.lowCannyThreshold = 50;
        codeDetector.markerDetector.highCannyThreshold = 100;
        UNIT_TEST(codeDetector.codes.size() == 1);
        
        image = imread("testfiles/QRCode/code19.png");
        codeDetector.markerDetector.lowCannyThreshold = 100;
        codeDetector.markerDetector.highCannyThreshold = 200;
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 1);        

        image = imread("testfiles/QRCode/code23.png");
        codeDetector.markerDetector.lowCannyThreshold = 200;
        codeDetector.markerDetector.highCannyThreshold = 400;
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 2);        

        image = imread("testfiles/QRCode/code31.jpg");
        codeDetector.markerDetector.lowCannyThreshold = 50;
        codeDetector.markerDetector.highCannyThreshold = 100;
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 3);

        image = imread("testfiles/QRCode/code61.jpg");
        codeDetector.markerDetector.lowCannyThreshold = 120;
        codeDetector.markerDetector.highCannyThreshold = 200;
        codeDetector.compute(image);
        UNIT_TEST(codeDetector.codes.size() == 6);
    }

    static double speed(unsigned long testCount) {
        QRCodeDetector codeDetector;
        Mat image;
        image = imread("testfiles/QRCode/code12.jpg");

        tic();
        for (unsigned long i = 0; i < testCount; i++) {
            codeDetector.compute(image);
        }
        return toc(testCount);
    }
};


#endif