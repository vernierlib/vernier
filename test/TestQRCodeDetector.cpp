/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "QRCodeDetector.hpp"
#include "UnitTest.hpp"
//#include <opencv2/objdetect/graphical_code_detector.hpp>

using namespace vernier;
using namespace std;
using namespace cv;

 void main1() {
        Mat image = imread("data/QRCode/code23.png");

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
    
//         void main2() { // test de la détection de QRCode avec opencv mais marche pas

//        cv::Mat grayImage, image = cv::imread("data/QRCode/code11.jpg");
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

//    }

    static void runAllTests() {
        
        START_UNIT_TEST;
        
        QRCodeDetector detector;
        Mat image;

        image = imread("data/QRCode/code11.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("data/QRCode/code12.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("data/QRCode/code13.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("data/QRCode/code14.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("data/QRCode/code15.jpg");
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("data/QRCode/code16.jpg");
        detector.fiducialDetector.lowCannyThreshold = 500;
        detector.fiducialDetector.highCannyThreshold = 600;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("data/QRCode/code17.jpg");
        detector.fiducialDetector.lowCannyThreshold = 100;
        detector.fiducialDetector.highCannyThreshold = 200;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);

        image = imread("data/QRCode/code18.jpg");
        erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)), Point(-1, -1), 3);
        detector.fiducialDetector.lowCannyThreshold = 50;
        detector.fiducialDetector.highCannyThreshold = 100;
        UNIT_TEST(detector.codes.size() == 1);
        
        image = imread("data/QRCode/code19.png");
        detector.fiducialDetector.lowCannyThreshold = 100;
        detector.fiducialDetector.highCannyThreshold = 200;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 1);        

        image = imread("data/QRCode/code23.png");
        detector.fiducialDetector.lowCannyThreshold = 200;
        detector.fiducialDetector.highCannyThreshold = 400;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 2);        

        image = imread("data/QRCode/code31.jpg");
        detector.fiducialDetector.lowCannyThreshold = 50;
        detector.fiducialDetector.highCannyThreshold = 100;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 3);

        image = imread("data/QRCode/code61.jpg");
        detector.fiducialDetector.lowCannyThreshold = 120;
        detector.fiducialDetector.highCannyThreshold = 200;
        detector.compute(image);
        UNIT_TEST(detector.codes.size() == 6);
    }

    static double speed(unsigned long testCount) {
        QRCodeDetector detector;
        Mat image;
        image = imread("data/QRCode/code12.jpg");

        tic();
        for (unsigned long i = 0; i < testCount; i++) {
            detector.compute(image);
        }
        return toc(testCount);
    }
    
int main(int argc, char** argv) {

//    main1();
    
//    cout << "Computing time: " << speed(100) << " ms" << endl;
    
    runAllTests();

    return EXIT_SUCCESS;
}