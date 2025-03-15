/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "HPCodePatternDetector.hpp"
#include "HPCodePatternLayout.hpp"
#include "UnitTest.hpp"
#include <iomanip>

using namespace vernier;
using namespace std;
using namespace cv;

void main1() {

    Mat image = cv::imread("data/QRCode/code31.jpg");

    HPCodePatternDetector estimator(15.5, 37, 256);

    estimator.detector.fiducialDetector.lowCannyThreshold = 50;
    estimator.detector.fiducialDetector.highCannyThreshold = 100;

    estimator.compute(image);
    estimator.draw(image);

    namedWindow("canny image", WINDOW_FREERATIO);
    imshow("canny image", estimator.detector.fiducialDetector.cannyImage);

    namedWindow("QR codes", WINDOW_FREERATIO);
    estimator.draw(image);
    imshow("QR codes", image);


    cout << "Found " << estimator.markers.size() << " markers" << endl;

    for (map<int, Pose>::iterator it = estimator.markers.begin(); it != estimator.markers.end(); it++) {
        cout << "QR code " << it->first << " at " << it->second.toString() << endl;
    }

    waitKey();
}

void main2() {

    Mat image = cv::imread("data/QRCode/code25.png");

    HPCodePatternDetector estimator(15.5, 33, 512);

    estimator.detector.fiducialDetector.lowCannyThreshold = 100;
    estimator.detector.fiducialDetector.highCannyThreshold = 200;

    bitwise_not(image, image);
    estimator.compute(image);
    bitwise_not(image, image);
    estimator.draw(image);

    imwrite("HPCode.png", image);

    imshow("QR codes", image);
    waitKey();
}

void main3() {

    ofstream file("data/Image140/results.csv");
    file << "image;no;x;y;alpha;pixelSize;no;x;y;alpha;pixelSize;" << endl;

    for (int i = 96; i <= 133; i++) {

        cout << "Loading image " << i << endl;
        Mat image = cv::imread("data/Image140/Image" + to_string(i) + ".png");
        //cv::Rect myROI(500, 500, 1500, 1000);
        //image = image(myROI);
        //bitwise_not(image, image);

        HPCodePatternDetector estimator(15.5, 33, 512);
        estimator.detector.fiducialDetector.lowCannyThreshold = 200;
        estimator.detector.fiducialDetector.highCannyThreshold = 400;
        estimator.compute(image);
        cout << "Found " << estimator.markers.size() << " markers" << endl;

        estimator.showControlImages();
        estimator.draw(image);
        imshow("QR codes", image);

        //        for (map<int, Pose>::iterator it = estimator.codes.begin(); it != estimator.codes.end(); it++) {
        //            cout << "QR code " << it->first << " at " << it->second.toString() << endl;
        //            file << it->first << ";" << it->second.x << ";" << it->second.y << ";" << it->second.alpha << ";" << it->second.pixelSize << ";";
        //        }
        if (estimator.markers.size() == 2) {
            file << i << ";";
            Pose p0 = estimator.get2DPose(0);
            file << 0 << ";" << p0.x << ";" << p0.y << ";" << p0.alpha << ";" << p0.pixelSize << ";";
            Pose p1 = estimator.get2DPose(1);
            file << 0 << ";" << p1.x << ";" << p1.y << ";" << p1.alpha << ";" << p1.pixelSize << ";";
            file << endl;
        }

        //waitKey(1);
    }
    file.close();
}

void test2d(int codeSize) {

    START_UNIT_TEST;

    // Constructing the layout
    double physicalPeriod = 8.0;
    PatternLayout* layout = new HPCodePatternLayout(physicalPeriod, 1 + (codeSize - 1) / 2);
    cout << "  Code size: " << codeSize << endl;
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
    //imshow("QR codes", image);
    //waitKey();

    // Detecting
    HPCodePatternDetector estimator(physicalPeriod, codeSize, 256);
    //estimator.detector.markerDetector.lowCannyThreshold = 100;
    //estimator.detector.markerDetector.highCannyThreshold = 200;
    //estimator.patternPhase.setSigma(1);
    estimator.compute(image);

    for (map<int, Pose>::iterator it = estimator.markers.begin(); it != estimator.markers.end(); it++) {
        cout << "  Marker " << it->first << " pose:" << it->second.toString() << endl;
        //it->second.draw(image);
    }


    // Drawing
    //estimator.showControlImages();


    drawCameraFrame(image);
    estimator.draw(image);

    //    imshow("HP codes", image);
    //    waitKey(1);

    if (estimator.markers.size() == 1) {
        Pose estimatedPose = estimator.markers.begin()->second;
        UNIT_TEST(areEqual(patternPose, estimatedPose, 0.1));
    }

}

void test(string filename, int lowCannyThreshold, int highCannyThreshold, int numberOfMarkers, int numberOfHalfPeriod, int snapshotSize) {

    START_UNIT_TEST;
    cv::Mat image = imread(filename);

    HPCodePatternDetector estimator = HPCodePatternDetector(15.5, numberOfHalfPeriod, snapshotSize);
    estimator.detector.fiducialDetector.lowCannyThreshold = lowCannyThreshold;
    estimator.detector.fiducialDetector.highCannyThreshold = highCannyThreshold;

    estimator.compute(image);
    cout << "  Found " << estimator.markers.size() << " markers" << endl;
    for (map<int, Pose>::iterator it = estimator.markers.begin(); it != estimator.markers.end(); it++) {
        cout << "  QR code " << it->first << " at " << it->second.toString() << endl;
    }

//    estimator.draw(image);
//    estimator.showControlImages();
//    imshow("QR codes", image);
//    waitKey(0);

    UNIT_TEST(estimator.markers.size() == numberOfMarkers);

}

void runAllTests() {
    test("data/QRCode/code17.jpg", 100, 200, 1, 37, 512);
    test("data/QRCode/code23.png", 200, 400, 2, 33, 512);
    test("data/QRCode/code24.png", 100, 300, 2, 33, 512);
    test("data/QRCode/code31.jpg", 50, 100, 3, 37, 256);
    test("data/QRCode/code61.jpg", 100, 210, 6, 37, 256);
    REPEAT_TEST(test2d(33), 10)
    REPEAT_TEST(test2d(37), 10)

}

double speed(unsigned long testCount) {
    Mat image = imread("data/QRCode/code31.jpg", -1);

    HPCodePatternDetector poses = HPCodePatternDetector(15.5, 37, 256);
    poses.detector.fiducialDetector.lowCannyThreshold = 50;
    poses.detector.fiducialDetector.highCannyThreshold = 100;

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        poses.compute(image);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {

    //    main1();

    //        main2();

    //    main3();

    //    cout << "Computing time: " << speed(100) << " ms" << endl;

    runAllTests();

    return EXIT_SUCCESS;
}