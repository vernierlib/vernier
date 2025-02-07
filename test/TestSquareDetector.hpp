/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#ifndef TESTSQUAREDETECTOR_HPP
#define TESTSQUAREDETECTOR_HPP

#include "SquareDetector.hpp"
#include "UnitTest.hpp"
#include <opencv2/objdetect/graphical_code_detector.hpp>

using namespace vernier;
using namespace std;
using namespace cv;

class TestSquareDetector {
public:

    static void main() {
#ifdef USE_OPENCV
        cv::Mat grayImage, image = cv::imread("testfiles/stamp/stamp2.png");
        if (image.channels() > 1) {
            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = image;
        }
        cv::normalize(grayImage, grayImage, 255, 0, cv::NORM_MINMAX);
        grayImage.convertTo(grayImage, CV_8UC1);

        SquareDetector detector;
        detector.compute(grayImage);
        
        cout << detector.toString() << endl;

        detector.draw(image);
        cv::imshow("Image", image);
        cv::waitKey(0);
#endif //USE_OPENCV 
    }
    
//        static void main2() { // test de la détection de QRCode avec opencv mais marche pas
//#ifdef USE_OPENCV
//        cv::Mat grayImage, image = cv::imread("testfiles/QRCode/code11.jpg");
//        if (image.channels() > 1) {
//            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
//        } else {
//            grayImage = image;
//        }
//        cv::normalize(grayImage, grayImage, 255, 0, cv::NORM_MINMAX);
//        grayImage.convertTo(grayImage, CV_8UC1);
// 
//        std::vector<std::vector<cv::Point> > corners;
//      
//        cv::GraphicalCodeDetector  detector;
//        detector.detectMulti(grayImage, corners);
// 
//        cv::imshow("Image", image);
//        cv::waitKey(0);
//#endif //USE_OPENCV 
//    }

    static void test(string filename, int numberOfMarkers) {
#ifdef USE_OPENCV
        START_UNIT_TEST;
        cv::Mat grayImage, image = cv::imread(filename);
        if (image.channels() > 1) {
            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = image;
        }
        cv::normalize(grayImage, grayImage, 255, 0, cv::NORM_MINMAX);
        grayImage.convertTo(grayImage, CV_8UC1);

        SquareDetector detector;
        detector.compute(grayImage);

        cout << "Found " << detector.squares.size() << " markers in " << filename << std::endl;

        UNIT_TEST(detector.squares.size() == numberOfMarkers);
#endif //USE_OPENCV            
    }

    static void runAllTests() {
        //        test("testfiles/QRCode/code11.jpg", 3);
        //        test("testfiles/QRCode/code12.jpg", 3);
        //        test("testfiles/QRCode/code13.jpg", 3);
        test("testfiles/QRCode/code14.jpg", 1);
        //        test("testfiles/QRCode/code15.jpg", 3);
        //        test("testfiles/QRCode/code16.jpg", 3);
        test("testfiles/QRCode/code17.jpg", 7);
        //        test("testfiles/QRCode/code21.jpg", 6);
        //        test("testfiles/QRCode/code22.tif", 6);
        //        test("testfiles/QRCode/code23.png", 6);
        //        test("testfiles/QRCode/code31.jpg", 9);
        //        test("testfiles/QRCode/code61.jpg", 18);
    }

    static double speed(unsigned long testCount) {
#ifdef USE_OPENCV
        SquareDetector detector;
        cv::Mat grayImage, image = cv::imread("testfiles/QRCode/code17.jpg");
        if (image.channels() > 1) {
            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = image;
        }
        cv::normalize(grayImage, grayImage, 255, 0, cv::NORM_MINMAX);
        grayImage.convertTo(grayImage, CV_8UC1);

        tic();
        for (unsigned long i = 0; i < testCount; i++) {
            detector.compute(grayImage);
        }
#endif //USE_OPENCV
        return toc(testCount);
    }
};


#endif