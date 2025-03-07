/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "QRFiducialDetector.hpp"
#include "UnitTest.hpp"

using namespace vernier;
using namespace std;
using namespace cv;

void main1() {

    cv::Mat image = cv::imread("data/QRCode/code24.png");

    QRFiducialDetector detector;

    detector.lowCannyThreshold = 100;
    detector.highCannyThreshold = 200;

    detector.compute(image);
    detector.drawRowPatterns(image);
    detector.drawColPatterns(image);
    detector.drawGroupsOfPatterns(image);
    detector.draw(image);
    cout << "Found " << detector.fiducials.size() << " fiducials." << std::endl;
    std::cout << detector.toString() << std::endl;

    cv::imshow("Edges", detector.cannyImage);
    cv::imshow("Image", image);
    cv::waitKey(0);

}

static void test(string filename, int lowCannyThreshold, int highCannyThreshold, int numberOfMarkers) {

    START_UNIT_TEST;
    cv::Mat image = imread(filename);

    QRFiducialDetector detector;

    detector.lowCannyThreshold = lowCannyThreshold;
    detector.highCannyThreshold = highCannyThreshold;

    detector.compute(image);
    cout << "Found " << detector.fiducials.size() << " fiducials in " << filename << std::endl;
    cout << detector.toString() << endl;

    UNIT_TEST(detector.fiducials.size() == numberOfMarkers);

}

double speed(unsigned long testCount) {

    QRFiducialDetector detector;
    cv::Mat image;
    image = cv::imread("data/QRCode/code12.jpg");
    detector.lowCannyThreshold = 50;
    detector.highCannyThreshold = 110;

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        detector.compute(image);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {
    
//    main1();
    
//    cout << "Computing time: " << speed(100) << " ms" << endl;
    
    test("data/QRCode/code11.jpg", 200, 400, 3);
    test("data/QRCode/code12.jpg", 200, 400, 3);
    test("data/QRCode/code13.jpg", 200, 400, 3);
    test("data/QRCode/code14.jpg", 200, 400, 3);
    test("data/QRCode/code15.jpg", 200, 400, 3);
    test("data/QRCode/code16.jpg", 350, 700, 3);
    test("data/QRCode/code17.jpg", 100, 200, 3);
    test("data/QRCode/code21.jpg", 200, 550, 6);
    test("data/QRCode/code22.tif", 50, 100, 6);
    test("data/QRCode/code23.png", 200, 400, 6);
    test("data/QRCode/code23.png", 100, 200, 6);
    test("data/QRCode/code31.jpg", 50, 110, 9);
    test("data/QRCode/code61.jpg", 100, 210, 18);

    return EXIT_SUCCESS;
}