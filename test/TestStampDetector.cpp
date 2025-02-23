/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "StampDetector.hpp"
#include "Layout.hpp"
#include "UnitTest.hpp"
#include <iomanip>

using namespace vernier;
using namespace std;
using namespace cv;

void main1() {

    string filename = "data/stamp/stamp2.png";
    Mat grayImage, image = imread(filename);
    imageTo8UC1(image, grayImage);

    StampDetector detector(15.5, 512, 61);
    detector.compute(grayImage);

    cout << "Found " << detector.stamps.size() << " markers in " << filename << endl;
    detector.draw(image);
    namedWindow("Image", WINDOW_FREERATIO);
    imshow("Image", image);

    detector.showControlImages();

    waitKey();
}

void testFile(string filename, int markerCount) {

    START_UNIT_TEST;

    cv::Mat grayImage, image = cv::imread(filename);
    imageTo8UC1(image, grayImage);

    StampDetector detector(15.5, 512, 61);
    detector.compute(grayImage);

    UNIT_TEST(detector.stamps.size() == markerCount);
}

void test2d() {

    START_UNIT_TEST;

    // Constructing the layout
    double physicalPeriod = randomDouble(15.0, 16.0)/2;
    PatternLayout* layout = new BitmapPatternLayout("data/stamp/stampF.png", physicalPeriod);
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(-80, 80);
    double y = randomDouble(-80, 80);
    double alpha = randomDouble(0, PI / 2);
    double pixelSize = 1.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose: " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(1024, 1024);
    layout->renderOrthographicProjection(patternPose, array);
    cv::Mat image = array2image(array);

    // Detecting
    Mat grayImage;
    imageTo8UC1(image, grayImage);
    StampDetector detector(physicalPeriod, 512, 69);
    detector.compute(grayImage);

    Pose estimatedPose;
    if (detector.stamps.size() == 1) {
        estimatedPose = detector.stamps[0];
        cout << "  Estimated pose: " << estimatedPose << endl;
        detector.draw(image);
    }

//    imshow("Image", image);
//    waitKey(1);

    UNIT_TEST(areEqual(patternPose, estimatedPose, 0.1));


}

double speed(unsigned long testCount) {

    // Constructing the layout
    double physicalPeriod = randomDouble(5.0, 6.0);
    PatternLayout* layout = new BitmapPatternLayout("data/stamp/stampF.png", physicalPeriod);
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(-80, 80);
    double y = randomDouble(-80, 80);
    double alpha = randomDouble(0, PI / 2);
    double pixelSize = 1.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose: " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(1024, 1024);
    layout->renderOrthographicProjection(patternPose, array);
    cv::Mat image = array2image(array);

    // Detecting
    Mat grayImage;
    imageTo8UC1(image, grayImage);
    StampDetector detector(physicalPeriod, 256, 61);
    detector.compute(grayImage);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        detector.compute(grayImage);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {

    //main1();

    //cout << "Computing time: " << speed(100) << " ms" << endl;

    REPEAT_TEST(test2d(), 20);

    testFile("data/stamp/stamp2.png", 2);

    return EXIT_SUCCESS;
}