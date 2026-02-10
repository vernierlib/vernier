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

void main2d() {
    // Constructing the layout
    double physicalPeriod = 15.0;
    PatternLayout* layout = new PeriodicPatternLayout(physicalPeriod, 31, 31);
    cout << "  Physical period: " << physicalPeriod << endl;
    
    // Setting the pose of the pattern in the camera frame for rendering
    double x = 6.0;
    double y = 3.0;
    double alpha = 0.2;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern pose: " << patternPose.toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Camera-to-pattern transformation matrix:" << endl << patternPose.getCameraToPatternTransformationMatrix() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern-to-camera transformation matrix:" << endl << patternPose.getPatternToCameraTransformationMatrix() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Dectecting and estimating the pose of the pattern
    PeriodicPatternDetector* detector;
    detector = new PeriodicPatternDetector(physicalPeriod);
    detector->setDouble("sigma", 1);
    detector->setDouble("cropFactor", 0.4);
    detector->compute(array);

    // Printing results 
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimation of pattern pose: " << detector->get2DPose().toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimation of camera-to-pattern transformation matrix:" << endl << detector->get2DPose().getCameraToPatternTransformationMatrix() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimation of pattern-to-camera transformation matrix:" << endl << detector->get2DPose().getPatternToCameraTransformationMatrix() << endl;

    // Showing image and is spectrum
    PatternPhase *patternPhase = (PatternPhase*) (detector->getObject("patternPhase"));
    arrayShow("Image", array);
    //arrayShow("Phase 1 (wrapped)", patternPhase->getPhase1());
    //arrayShow("Phase 2 (wrapped)", patternPhase->getPhase2());
    //imshow("Spectrum", array2image(patternPhase->getSpectrum()));
    // imshow("spectrumFiltered1", array2image(patternPhase->getSpectrumPeak1()));
    // imshow("spectrumFiltered2", array2image(patternPhase->getSpectrumPeak2()));

    detector->showControlImages();
    //imshow("Spectrum GUI", patternPhase->getControlImage());
    waitKey();
}

void main3dPerspective() {

    // Constructing the layout
    double physicalPeriod = 15;
    PatternLayout* layout = new PeriodicPatternLayout(physicalPeriod, 31, 31);
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = 0 ;
    double y = 0 ;
    double z = 600.0;
    double alpha = 0.0;
    double beta = 0.30;
    double gamma = 0.00;
    double pixelSize = 1.0;
    Pose patternPose = Pose(x, y, z, alpha, beta, gamma);
    cout << "  Pattern pose:     " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    double focalLength = 100/0.5;
    layout->renderPerspectiveProjection(patternPose, array, focalLength);

    // Detecting and estimating the pose of the pattern
    PeriodicPatternDetector* detector;
    detector = new PeriodicPatternDetector(physicalPeriod);
    detector->setDouble("sigma", 5);
    detector->setDouble("cropFactor", 0.8);
    detector->compute(array);
      
    Pose pose = detector->get3DPosePerspective(focalLength);
    cout << "  Estimated pose: " << pose.toString() << endl;
    detector->showControlImages();
    
    // Detecting and estimating the pose of the pattern
    cv::Mat cameraMatrix = (Mat_<double>(3, 3) << focalLength / pixelSize, 0.0, array.cols()/2.0, 0.0, focalLength / pixelSize, array.rows()/2.0, 0.0, 0.0, 1.0);
    cv::Mat distortionCoefficients = (Mat_<double>(1, 5) << 0.0, 0.0, 0.0, 0.0, 0.0);
    cv::Mat rvec;
    cv::Mat tvec;  
    detector->get3DPosePerspective(cameraMatrix, distortionCoefficients, rvec, tvec);
    cout << "  Estimated tvec: " << tvec.t() << endl;  
    cout << "  Estimated rvec: " << rvec.t() << endl;   
    //cout << "  Matrix : " << cameraMatrix << endl;  
    
    cv::Mat image;
    array2image8UC4(array, image);
    tvec = (Mat_<double>(1, 3) << 0.0, 0.0, 0.0);
    cv::drawFrameAxes(image, cameraMatrix, distortionCoefficients, rvec, tvec, 10.0);
    cv::imshow("Image", image);
    
    
    waitKey(0);
}

void test2d() {
    START_UNIT_TEST;
    
    // Constructing the layout
    double physicalPeriod = randomDouble(5.0, 10.0);
    PatternLayout* layout = new PeriodicPatternLayout(physicalPeriod, 81, 61);
    cout << "  Physical period: " << physicalPeriod << endl;
    
    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(0.0, physicalPeriod / 2.01);
    double y = randomDouble(0.0, physicalPeriod / 2.01);
    double alpha = randomDouble(0, PI / 2);
    double pixelSize = randomDouble(1.0, 1.1);
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose:   " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);
    //            arrayShow(layout->getDescription(), array);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = new PeriodicPatternDetector(physicalPeriod);
    detector->compute(array);

    // Printing results 
    Pose estimatedPose;
    if (detector->patternFound()) {
        estimatedPose = detector->get2DPose();
        cout << "  Estimated pose: " << estimatedPose.toString() << endl;
    } else {
        cout << "Pattern not found." << endl;
    }

    TEST_EQUALITY(patternPose, estimatedPose, 0.01)
}

int main(int argc, char** argv) {

    //main2d();

    main3dPerspective();
    
    //REPEAT_TEST(test2d(), 10)

    return EXIT_SUCCESS;
}