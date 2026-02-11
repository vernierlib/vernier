/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Vernier.hpp"
#include "UnitTest.hpp"

using namespace vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

void main1() {

    MegarenaPatternDetector detector(9, 12);
    cout << detector.toString() << endl;

    cv::Mat image = cv::imread("data/megarena12bits.jpg");
    detector.compute(image);

    detector.showControlImages();

    std::cout << "Camera pose in Pattern" << detector.get2DPose().toString() << std::endl;
    detector.get2DPose().draw(image);
    imshow("Image", image);
    waitKey(0);

}

void example2d() {
    // Loading the layout
    double physicalPeriod = 10.0;
    int codeSize = 12;
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);

    // Setting the pose for rendering
    double x = -15 * 400.0;
    double y = -15 * 200.0;
    double alpha = 0.2;
    double pixelSize = 1.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    std::cout << "------------------------------------------------------------------" << endl;
    std::cout << "Pattern pose:   " << patternPose.toString() << endl;

    // Rendering
    ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);
    //arrayShow("Image", array);
    //waitKey(0);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
    detector->compute(array);

    // Printing results 
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose: " << detector->get2DPose().toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Camera-to-pattern transformation matrix:" << endl << detector->get2DPose().getCameraToPatternTransformationMatrix() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern-to-camera transformation matrix:" << endl << detector->get2DPose().getPatternToCameraTransformationMatrix() << endl;
    cout << "------------------------------------------------------------------" << endl;

    // Showing image and its spectrum
    detector->showControlImages();
    waitKey(0);
}

/** This example renders an image of a megarena pattern and estimates its 3D pose 
 * with an orthographic projection
 */
void example3d() {

    // Loading the layout
    double physicalPeriod = 10.0;
    int codeSize = 12;
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);

    // Setting the pose for rendering
    double x = -15 * 400.0;
    double y = -15 * 200.0;
    double alpha = 0.2;
    double beta = 0.3;
    double gamma = 0.4;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern pose:     " << patternPose.toString() << endl;

    // Rendering
    ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Estimating the pose of the pattern
    PatternDetector* detector;
    detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
    detector->compute(array);

    // Printing results 
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose 0: " << detector->getAll3DPoses()[0].toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose 1: " << detector->getAll3DPoses()[1].toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose 2: " << detector->getAll3DPoses()[2].toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose 3: " << detector->getAll3DPoses()[3].toString() << endl;
    cout << "------------------------------------------------------------------" << endl;

    // Showing image and is spectrum
    detector->showControlImages();

}

/** This example captures an image of a megarena pattern and estimates its 2D pose 
 */
int exampleCapture() {

    // Initializing VideoCapture
    VideoCapture cap;
    int deviceID = 0; // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    cap.open(deviceID, apiID);
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    // Waiting for a new frame from camera
    Mat frame;
    cap.read(frame);
    if (frame.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
        return -1;
    }
    imshow("Camera", frame);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = Detector::loadFromJSON("megarenaPattern.json");
    detector->compute(frame);

    // Printing results 
    cout << "Estimated pose: " << detector->get2DPose().toString() << endl;

    // Showing image and its spectrum
    detector->showControlImages();

    return 0;
}

void example2dbis() {

    // Constructing the layout
    double physicalPeriod = randomDouble(5.0, 10.0);
    int codeSize = 12;
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
    cout << "  Code size: " << codeSize << endl;
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = -300 * 15.0;
    double y = -100 * 15.0;
    double alpha = -0.2;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose:   " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
    detector->compute(array);
    Pose estimatedPose = detector->get2DPose();

    // Printing results 
    cout << "  Estimated pose: " << estimatedPose.toString() << endl;
    detector->showControlImages();
    waitKey(0);
}

void main3d() {

    // Constructing the layout
    double physicalPeriod = randomDouble(5.0, 10.0);
    int codeSize = 12;
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
    cout << "  Code size: " << codeSize << endl;
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(-layout->getWidth() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
    double y = randomDouble(-layout->getHeight() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
    double alpha = randomDouble(-PI, PI);
    double beta = randomDouble(-0.5, 0.5);
    double gamma = randomDouble(-0.5, 0.5);
    double pixelSize = randomDouble(1.0, 1.10);
    Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);
    cout << "  Pattern pose:     " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
    detector->compute(array);
    std::vector<Pose> estimatedPoses = detector->getAll3DPoses();

    // Printing results 
    cout << "  Estimated pose 0: " << estimatedPoses[0].toString() << endl;
    cout << "  Estimated pose 1: " << estimatedPoses[1].toString() << endl;
    cout << "  Estimated pose 2: " << estimatedPoses[2].toString() << endl;
    cout << "  Estimated pose 4: " << estimatedPoses[3].toString() << endl;
    detector->showControlImages();
    waitKey(0);
}

void main3dLongFocalPerspective() {

    // Constructing the layout
    double physicalPeriod = 15.0;
    int codeSize = 12;
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
    cout << "  Code size: " << codeSize << endl;
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = -200 * 15.0;
    double y = -300 * 15.0;
    double z = 5000.0;
    double alpha = -0.2;
    double beta = 0.10;
    double gamma = 0.05;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, z, alpha, beta, gamma, pixelSize);
    cout << "  Pattern pose:     " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderPerspectiveProjection(patternPose, array, 5000.0);

    // Detecting and estimating the pose of the pattern
    MegarenaPatternDetector* detector;
    detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
    detector->setDouble("sigma", 5);
    detector->setDouble("cropFactor", 0.8);
    detector->compute(array);
    std::vector<Pose> estimatedPoses = detector->getAll3DPoses();

    // Printing results 
    cout << "  Estimated pose 0: " << estimatedPoses[0].toString() << endl;
    cout << "  Estimated pose 1: " << estimatedPoses[1].toString() << endl;
    cout << "  Estimated pose 2: " << estimatedPoses[2].toString() << endl;
    cout << "  Estimated pose 4: " << estimatedPoses[3].toString() << endl;
    detector->showControlImages();
    waitKey(0);
}

void test2d(int codeSize) {
    START_UNIT_TEST;

    // Constructing the layout
    double physicalPeriod = randomDouble(5.0, 10.0);
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
    cout << "  Code size: " << codeSize << endl;
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(-layout->getWidth() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
    double y = randomDouble(-layout->getHeight() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
    double alpha = randomDouble(-PI, PI);
    double pixelSize = randomDouble(1.0, 1.1);
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose:   " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
    detector->compute(array);
    Pose estimatedPose = detector->get2DPose();

    // Printing results 
    cout << "  Estimated pose: " << estimatedPose.toString() << endl;
    //            detector->showControlImages();
    //            waitKey(0);

    TEST_EQUALITY(patternPose, estimatedPose, 0.01)
}

void test3d(int codeSize) {
    START_UNIT_TEST;

    // Constructing the layout
    double physicalPeriod = randomDouble(5.0, 10.0);
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
    cout << "  Code size: " << codeSize << endl;
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(-layout->getWidth() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
    double y = randomDouble(-layout->getHeight() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
    double alpha = randomDouble(-PI, PI);
    double beta = randomDouble(-0.5, 0.5);
    double gamma = randomDouble(-0.5, 0.5);
    double pixelSize = randomDouble(1.0, 1.10);
    Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);
    cout << "  Pattern pose:     " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
    detector->compute(array);
    std::vector<Pose> estimatedPoses = detector->getAll3DPoses();

    // Printing results 
    cout << "  Estimated pose 0: " << estimatedPoses[0].toString() << endl;
    cout << "  Estimated pose 1: " << estimatedPoses[1].toString() << endl;
    cout << "  Estimated pose 2: " << estimatedPoses[2].toString() << endl;
    cout << "  Estimated pose 4: " << estimatedPoses[3].toString() << endl;
    //            detector->showControlImages();
    //            waitKey(0);

    UNIT_TEST(areEqual(patternPose, estimatedPoses[0], 0.1)
            || areEqual(patternPose, estimatedPoses[1], 0.1)
            || areEqual(patternPose, estimatedPoses[2], 0.1)
            || areEqual(patternPose, estimatedPoses[3], 0.1))
}

void runAllTests() {
    REPEAT_TEST(test2d(8), 10)
    REPEAT_TEST(test2d(10), 10)
    REPEAT_TEST(test2d(12), 10)
    REPEAT_TEST(test3d(8), 10);
}

double speed(unsigned long testCount) {

    // Constructing the layout
    double physicalPeriod = 15.0;
    int codeSize = 12;
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);

    // Setting the pose of the pattern in the camera frame for rendering
    double x = -300 * 15.0;
    double y = -100 * 15.0;
    double alpha = -0.2;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);

    // Rendering
    Eigen::ArrayXXd array(1024, 1024);
    layout->renderOrthographicProjection(patternPose, array);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = new MegarenaPatternDetector(physicalPeriod, 12);
    detector->compute(array);
    Pose estimatedPose = detector->get2DPose();

    tic();
    for (unsigned long i = 0; i < testCount; i++) {
        detector->compute(array);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {
    
    //example2d();
    //main1();
    //main3d();
    
//    cout << "Computing time: " << speed(10) << " ms" << endl;
  
    runAllTests();

    return EXIT_SUCCESS;
}