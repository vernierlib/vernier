/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "Detector.hpp"
#include "BitmapPatternDetector.hpp"
#include "Layout.hpp"
#include "UnitTest.hpp"
#include <iomanip>

using namespace vernier;
using namespace std;
using namespace cv;

void main1() {

    //    HPCodePatternLayout layout(2, 19);
    //    layout.saveToPNG("data/HPCode37x37.png");

    BitmapPatternDetector detector(2, "data/HPCode37x37.png");
    cout << detector.toString() << endl;

    cv::Mat image = cv::imread("data/QRCode/code18_270.jpg");
    detector.compute(image);

    detector.showControlImages();

    std::cout << "Camera pose in Pattern" << detector.get2DPose().toString() << std::endl;
    detector.get2DPose().draw(image);
    imshow("Image", image);
    waitKey(0);

}

void main2() {

    //    BitmapPatternLayout layout5("data/HPCode33x33.png", 2);
    //    layout5.saveToJSON("BitmapPatternDetector.json");

    PatternDetector* detector;
    detector = Detector::loadFromJSON("BitmapPatternDetector.json");
    cout << detector->toString() << endl;

    cv::Mat image = cv::imread("data/QRCode/code18.jpg");
    detector->compute(image);

    detector->showControlImages();

    std::cout << "Camera pose in Pattern" << detector->get2DPose().toString() << std::endl;
    detector->get2DPose().draw(image);
    imshow("Image", image);
    waitKey(0);

}

void test2d(const string & filename) {

    START_UNIT_TEST;

    // Constructing the layout
    double physicalPeriod = randomDouble(4., 8.0);
    PatternLayout* layout = new BitmapPatternLayout(filename, physicalPeriod);
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(-90, 90);
    double y = randomDouble(-90, 90);
    double alpha = randomDouble(-PI, PI);
    double pixelSize = randomDouble(1.0, 1.1);
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose:   " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);
    
    // Detecting
    BitmapPatternDetector detector(physicalPeriod, filename);
    detector.compute(array);

    // Printing results 
    Pose estimatedPose;
    if (detector.patternFound()) {
        estimatedPose = detector.get2DPose();
        cout << "  Estimated pose: " << estimatedPose << endl;

        // Drawing
//        cv::Mat image = array2image(array);
//        drawCameraFrame(image);
//        detector.get2DPose().draw(image);
//        detector.showControlImages();
//        imshow("HP codes", image);
//        waitKey(10);
    } else {
        cout << "Pattern not found." << endl;
    }

    UNIT_TEST(areEqual(patternPose, estimatedPose, 0.5));
}

int main(int argc, char** argv) {

    //main1();

    //cout << "Computing time: " << speed(100) << " ms" << endl;

    REPEAT_TEST(test2d("data/HPCode37x37.png"), 20);
    
    REPEAT_TEST(test2d("data/femto117x45.png"), 20);

    return EXIT_SUCCESS;
}