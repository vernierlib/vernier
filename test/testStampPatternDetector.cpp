/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "StampPatternDetector.hpp"
#include "Layout.hpp"
#include "UnitTest.hpp"
#include <iomanip>
#include <opencv4/opencv2/core/mat.hpp>

using namespace vernier;
using namespace std;
using namespace cv;

void main1() {
    
    string filename = "data/stamp/stamp3.png";
    Mat image = imread(filename);
    
    StampPatternDetector detector(15.5, "data/stamp/stampD.png", 820);
    detector.addBitmap("data/stamp/stampG.png");
    
    detector.setPrincipalPoint(image.cols/2, image.rows/2);
    detector.setFocalLength(1.0);
    
    detector.compute(image);
    detector.showControlImages();

    cout << "Found " << detector.markers.size() << " markers in " << filename << endl;
    for (map<int, Pose>::iterator it = detector.markers.begin(); it != detector.markers.end(); it++) {
        cout << "Stamp " << it->first << " at " << it->second.toString() << endl;
    }
    
    detector.draw(image);
    namedWindow("Image", WINDOW_FREERATIO);
    imshow("Image", image);
   
    waitKey();
}

//void testFile(string filename, int markerCount) {
//
//    START_UNIT_TEST;
//
//    cv::Mat grayImage, image = cv::imread(filename);
//    imageTo8UC1(image, grayImage);
//
//    StampPatternDetector detector(15.5, 512, 61);
//    detector.compute(grayImage);
//
//    UNIT_TEST(detector.stamps.size() == markerCount);
//}

void test2d() {

    START_UNIT_TEST;

    // Constructing the layout
    double physicalPeriod = randomDouble(7.0, 8.0);
    PatternLayout* layout = new BitmapPatternLayout("data/stamp/stamp+.png", physicalPeriod);
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(-200, 200);
    double y = randomDouble(-200, 200);
    double alpha = randomDouble(-PI, PI);
    double pixelSize = randomDouble(1.0, 1.5);
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose: " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(1024, 1024);
    layout->renderOrthographicProjection(patternPose, array);

    // Detecting
    StampPatternDetector detector(physicalPeriod, "data/stamp/stamp+.png", 420);
    detector.setPrincipalPoint(array.cols()/2, array.rows()/2);
    detector.setFocalLength(1.0);
    detector.compute(array);

    Pose estimatedPose;
    if (detector.patternFound()) {
        estimatedPose = detector.get2DPose();//
        cout << "  Estimated pose: " << estimatedPose << endl;      
    }
    
//    cv::Mat image;
//    array2image8UC4(array, image);
//    detector.showControlImages();
//    vernier::drawCameraFrame(image);
//    detector.draw(image);
//    imshow("Image", image);
//    waitKey(10);

    UNIT_TEST(areEqual(patternPose, estimatedPose, 0.2));


}

void test3d() {

    START_UNIT_TEST;

    // Constructing the layout
    double physicalPeriod = 8;//randomDouble(7.0, 8.0);
    PatternLayout* layout = new BitmapPatternLayout("data/stamp/stamp+.png", physicalPeriod);
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = 0;//randomDouble(-200, 200);
    double y = 0;//randomDouble(-200, 200);
    double z = 200;
    double alpha = 0.2;//randomDouble(-PI, PI);
    double beta = 0.1;//randomDouble(-PI, PI);
    double pixelSize = 0.1;//randomDouble(1.0, 1.5);
    Pose patternPose = Pose(x, y, z, alpha, beta, 0.0, pixelSize);
    cout << "  Pattern pose: " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(1024, 1024);
    //layout->renderOrthographicProjection(patternPose, array);
    double focalLength = 20;
    layout->renderPerspectiveProjection(patternPose, array, focalLength);

    // Detecting
    StampPatternDetector detector(physicalPeriod, "data/stamp/stamp+.png", 420);
    detector.setPrincipalPoint(array.cols()/2, array.rows()/2);
    detector.setFocalLength(1.0);
    detector.compute(array);

    Pose estimatedPose;
    if (detector.patternFound()) {
        estimatedPose = detector.get2DPose();//
        cout << "  Estimated pose: " << estimatedPose << endl;   
        
        
        cv::Mat cameraMatrix = (Mat_<double>(3, 3) << focalLength / pixelSize, 0.0, array.cols()/2.0, 0.0, focalLength / pixelSize, array.rows()/2.0, 0.0, 0.0, 1.0);
        cv::Mat distortionCoefficients = (Mat_<double>(1, 5) << 0.0, 0.0, 0.0, 0.0, 0.0);
        cv::Mat rvec;
        cv::Mat tvec;  
        detector.get3DPose(cameraMatrix, distortionCoefficients, rvec, tvec);
        
        cout << "  Estimated tvec: " << tvec.t() << endl;  
        cout << "  Estimated rvec: " << rvec.t() << endl;   
        cout << "  Matrix : " << cameraMatrix << endl;  
        
        
        
    }
    
    cv::Mat image;
    array2image8UC4(array, image);
    detector.showControlImages();
    vernier::drawCameraFrame(image);
    detector.draw(image);
    cv::imshow("Image", image);
    cv::waitKey(0);

    UNIT_TEST(areEqual(patternPose, estimatedPose, 0.2));


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
    cv::Mat image;
    array2image8UC4(array, image);

    // Detecting
    StampPatternDetector detector(physicalPeriod, "data/stamp/stampF.png", 512);
    detector.compute(array);

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        detector.compute(array);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {

    //main1();

    //cout << "Computing time: " << speed(100) << " ms" << endl;

    REPEAT_TEST(test3d(), 20);

    //testFile("data/stamp/stamp2.png", 2);

    return EXIT_SUCCESS;
}