/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "Detector.hpp"
#include "Layout.hpp"
#include "UnitTest.hpp"
#include <iomanip>

using namespace vernier;
using namespace std;
using namespace cv;

void main1() {

    //    HPCodePatternLayout layout(18, 17);
    //    layout.saveToPNG("data/HPCode33.png");

    //    BitmapPatternLayout layout5("data/HPCode33.png", 2);
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

//void mainBitmapFullStack() {
//    std::cout << "BITMAP PATTERN LAYOUT AND DETECTOR FULL TEST" << std::endl;
//    std::cout << "JSON Bitmap pattern file generation" << std::endl;
//    BitmapPatternLayout layout("TestBitmapPatternUnit.png", 9);
//
//    layout.saveToJSON("testBitmapPattern.json");
//    std::cout << "done" << std::endl;
//
//    std::cout << "Periodic pattern rendering" << std::endl;
//    PatternLayout* patternLayout;
//    patternLayout = Layout::loadFromJSON("testBitmapPattern.json");
//
//    
//    Eigen::ArrayXXd array(1024, 1024);
//
//    layout.renderOrthographicProjection(Pose(-10, -5, 1000, 0.23, 0.0, 0.0), array);
//    std::cout << "done" << std::endl;
//
//    cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());
//
//    PatternDetector* mDetect;
//    mDetect = Detector::loadFromJSON("testBitmapPattern.json");
//
//    mDetect->compute(image);
//    Vernier::Pose patternPose = mDetect->getCameraPoseInPattern();
//    std::cout << "Periodic pattern detector poses :" << std::endl;
//    std::cout << "Camera pose in Pattern" << patternPose.toString() << std::endl;
//    std::cout << "Pattern pose in Camera" << mDetect->getPatternPoseInCamera().toString() << std::endl;
//
//
//    cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
//    cv::imshow(patternLayout->getClassname(), image);
//    cv::waitKey();
//}

int main(int argc, char** argv) {

    main1();

    //cout << "Computing time: " << speed(100) << " ms" << endl;

    //REPEAT_TEST(test2d(), 20);

    //testFile("data/stamp/stamp2.png", 2);

    return EXIT_SUCCESS;
}