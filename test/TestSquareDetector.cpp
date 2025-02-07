/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "SquareDetector.hpp"
#include "UnitTest.hpp"

using namespace vernier;
using namespace std;
using namespace cv;

void main1() {

    string filename = "testfiles/stamp/stamp2.png";
    Mat grayImage, image = imread(filename);
    imageTo8UC1(image, grayImage);

    SquareDetector detector;
    detector.compute(grayImage);
    cout << "Found " << detector.squares.size() << " squares in " << filename << endl;
    cout << "Squares: " << detector.toString() << endl;

    detector.draw(image);
    cv::imshow("Image", image);
    cv::waitKey(0);
}

void test(string filename, int numberOfMarkers) {
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

    cout << "    Found " << detector.squares.size() << " squares in " << filename << endl;

    UNIT_TEST(detector.squares.size() == numberOfMarkers);
}

double speed(unsigned long testCount) {

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

    return toc(testCount);
}

int main(int argc, char** argv) {

    main1();
    
    cout << "Computing time: " << speed(100) << " ms" << endl;

    test("testfiles/QRCode/code14.jpg", 1);
    test("testfiles/QRCode/code17.jpg", 7);

    return EXIT_SUCCESS;
}