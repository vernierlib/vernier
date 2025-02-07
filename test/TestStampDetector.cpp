/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "StampDetector.hpp"
#include "QRCodePatternLayout.hpp"
#include "UnitTest.hpp"
#include <iomanip>

using namespace vernier;
using namespace std;
using namespace cv;

void example1() {

    cv::Mat grayImage, image = cv::imread("testfiles/stamp/stamp2.png");
    imageTo8UC1(image, grayImage);

    StampDetector detector(15.5, 512, 61);

    detector.compute(grayImage);
    detector.draw(image);

    namedWindow("Image", WINDOW_FREERATIO);
    imshow("Image", image);

    detector.showControlImages();

    cout << "Found " << detector.stamps.size() << " markers" << endl;

    waitKey();
}

void test2d() {

    // Constructing the layout
    double physicalPeriod = 7.0;
    PatternLayout* layout = new QRCodePatternLayout(physicalPeriod, 1 + (37 - 1) / 2);
    
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(-80, 80);
    double y = randomDouble(-80, 80);
    double alpha = randomDouble(-PI, PI);
    double pixelSize = 1.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose: " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);
    cv::Mat image = array2image(array);
    imshow("QR codes", image);
    waitKey();

    // Detecting
    
    StampDetector detector(physicalPeriod, 64, 37);

    detector.compute(image);
    detector.draw(image);
    
//    HPCodeDetector estimator(physicalPeriod, 256, codeSize);
//    //estimator.detector.markerDetector.lowCannyThreshold = 100;
//    //estimator.detector.markerDetector.highCannyThreshold = 200;
//    //estimator.patternPhase.setSigma(1);
//    estimator.compute(image);
//
//    for (map<int, Pose>::iterator it = estimator.codes.begin(); it != estimator.codes.end(); it++) {
//        cout << "  Marker " << it->first << " pose:" << it->second.toString() << endl;
//        //it->second.draw(image);
//    }
//    // Drawing
//    estimator.showControlImages();
//    drawCameraFrame(image);
//    estimator.drawPose(image);
//
//    imshow("QR codes", image);
//    moveWindow("QR codes", 256, 0);
//
//    waitKey(1);
//
//    if (estimator.codes.size() == 1) {
//        Pose estimatedPose = estimator.codes.begin()->second;
//        UNIT_TEST(areEqual(patternPose, estimatedPose, 0.5));
//    }

}

void runAllTests() {
    //        test("testfiles/QRCode/code17.jpg", 100, 200, 1, 512, 37);
    //        test("testfiles/QRCode/code23.png", 200, 400, 2, 512, 33);
    //        test("testfiles/QRCode/code31.jpg", 50, 100, 3, 256, 37);
    //        test("testfiles/QRCode/code61.jpg", 100, 210, 6, 256, 37);
    //        REPEAT_TEST(test2d(33), 10)
    //        REPEAT_TEST(test2d(37), 10)

}

double speed(unsigned long testCount) {
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
    return -1;
}

int main(int argc, char** argv) {

    example1();
    //test2d();
    //cout << "Computing time: " << TestHPCodeDetector::speed(100) << " ms" << endl;
    //TestHPCodeDetector::runAllTests();

    return EXIT_SUCCESS;
}