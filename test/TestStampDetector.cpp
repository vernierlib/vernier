/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "StampDetector.hpp"
#include "QRCodePatternLayout.hpp"
#include "UnitTest.hpp"
#include <iomanip>

using namespace Vernier;
using namespace std;
using namespace cv;

class TestStampDetector {
public:

    static void main() {

        cv::Mat grayImage, image = cv::imread("testfiles/stamp/stamp2.png");
        if (image.channels() > 1) {
            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = image;
        }
        cv::normalize(grayImage, grayImage, 255, 0, cv::NORM_MINMAX);
        grayImage.convertTo(grayImage, CV_8UC1);

        StampDetector detector(15.5, 512, 61);
        
        detector.compute(grayImage);
        detector.draw(image);

        namedWindow("Image", WINDOW_FREERATIO);
        imshow("Image", image);

        detector.showControlImages();

        cout << "Found " << detector.stamps.size() << " markers" << endl;

        waitKey();
    }

    static void runAllTests() {
//        test("testfiles/QRCode/code17.jpg", 100, 200, 1, 512, 37);
//        test("testfiles/QRCode/code23.png", 200, 400, 2, 512, 33);
//        test("testfiles/QRCode/code31.jpg", 50, 100, 3, 256, 37);
//        test("testfiles/QRCode/code61.jpg", 100, 210, 6, 256, 37);
//        REPEAT_TEST(test2d(33), 10)
//        REPEAT_TEST(test2d(37), 10)

    }

    static double speed(unsigned long testCount) {
//        Mat image = imread("testfiles/QRCode/code31.jpg", -1);
//
//        HPCodeDetector poses = HPCodeDetector(15.5, 256, 37);
//        poses.detector.fiducialDetector.lowCannyThreshold = 50;
//        poses.detector.fiducialDetector.highCannyThreshold = 100;
//
//        tic();
//        for (unsigned long i = 0; i < testCount; i++) {
//            poses.compute(image);
//        }
//
//        return toc(testCount);
    }
};

int main(int argc, char** argv) {

    TestStampDetector::main();
    //TestHPCodeDetector::main2();
    //cout << "Computing time: " << TestHPCodeDetector::speed(100) << " ms" << endl;
    //TestHPCodeDetector::runAllTests();

    return EXIT_SUCCESS;
}