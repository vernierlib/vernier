/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#pragma once

#include "Vernier.hpp"

using namespace Vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

namespace Vernier {

    class TestPeriodicPatternDetector {
    public:

        static void main2d() {
            // Loading the layout
            string filename = "periodicPattern.json";
            PatternLayout* layout = Layout::loadFromJSON(filename);

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
            PatternDetector* detector;
            detector = Detector::loadFromJSON(filename);
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
            imshow(layout->getDescription() + " (" + filename + ")", array2image(array));
            imshow("Phase 1 (wrapped)", array2image(patternPhase->getPhase1()));
            imshow("Phase 2 (wrapped)", array2image(patternPhase->getPhase2()));
            //imshow("Spectrum", array2image(patternPhase->getSpectrum()));
           // imshow("spectrumFiltered1", array2image(patternPhase->getSpectrumPeak1()));
           // imshow("spectrumFiltered2", array2image(patternPhase->getSpectrumPeak2()));
            
            detector->showControlImages();
            //imshow("Spectrum GUI", patternPhase->getControlImage());
            waitKey();
        }

        static void main3d() {
            //            // Loading the layout
            //            string filename = "periodicPattern.json";
            //            PatternLayout* layout = Layout::loadFromJSON(filename);
            //
            // Setting the pose of the pattern in the camera frame for rendering
//    srand(time(NULL));
//    double x = 7.0 * rand() / RAND_MAX; // 6.0
//    double y = 7.0 * rand() / RAND_MAX; // 3.0
//    double alpha = 0.2 * PI * rand() / RAND_MAX; // 0.3;
//    double beta = 0.2 * PI * rand() / RAND_MAX; // 0.2;
//    double gamma = 0.2 * PI * rand() / RAND_MAX; // 0.4;
//    double pixelSize = 2.0;
//    Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);
            //            // Setting the pose of the pattern in the camera frame for rendering
            //            double x = 5.0;
            //            double y = 4.0;
            //            double z = 1000.0;
            //            double rz = 0.0;
            //            double ry = 0.3;
            //            double rx = 0.0;
            //            Eigen::Matrix4d patternPose = pose3d(x, y, z, rz, ry, rx);
            //
            //            // Allocating an array for rendering
            //            Eigen::ArrayXXd array(512, 512);
            //
            //            // Rendering
            //            double pixelSize = 1.0;
            //            double focalLength = 1000.0;
            //            PatternRenderer patternRenderer(layout, pixelSize, focalLength);
            //            patternRenderer.renderOrthographicProjection(patternPose, array);
            //
            //            // Dectecting and estimating the pose of the pattern
            //            PatternDetector* detector;
            //            detector = Detector::loadFromJSON(filename);
            //            detector->computePerspective(array);
            //
            //            // Printing results 
            //            // cout << "Pattern pose in camera frame for rendering : " << endl << patternPose.getCameraPatternTransformation() << endl;
            //            cout << "Pattern pose in camera frame for rendering : " << endl << patternPose << endl;
            //            //  cout << "Estimation of pattern pose in camera frame : " << endl << detector->getPatternPose() << endl;
            //            cout << "------------------------------------------------------------------" << endl;
            //            std::vector<Pose> tab = detector->get3DPose();
            //            cout << "Estimation of pattern pose in camera frame : " << endl << tab[0].getCameraPatternTransformation() << endl;
            //            cout << "------------------------------------------------------------------" << endl;
            //            cout << "Estimation of pattern pose in camera frame : " << endl << tab[1].getCameraPatternTransformation() << endl;
            //            cout << "------------------------------------------------------------------" << endl;
            //            cout << "Estimation of pattern pose in camera frame : " << endl << tab[2].getCameraPatternTransformation() << endl;
            //            cout << "------------------------------------------------------------------" << endl;
            //            cout << "Estimation of pattern pose in camera frame : " << endl << tab[3].getCameraPatternTransformation() << endl;
            //
            //
            //            // Converting the array in opencv mat and showing it
            //            Mat image(array.cols(), array.rows(), CV_64F, array.data());
            //            transpose(image, image);
            //            drawCameraFrame(image);
            //            imshow(layout->getDescription() + " (" + filename + ")", image);
            //            imshow("Spectrum", detector->getGuiSpectrum());
            //            waitKey();
        }
        
        static void test2d() {
            // Constructing the layout
            PatternLayout* layout = new PeriodicPatternLayout(15,81,61);

            // Setting the pose of the pattern in the camera frame for rendering
            double x = 6.0;
            double y = 3.0;
            double alpha = 0.2;
            double pixelSize = 2.0;
            Pose patternPose = Pose(x, y, alpha, pixelSize);
             cout << "Pattern pose: " << patternPose.toString() << endl;
            
            // Rendering
            Eigen::ArrayXXd array(512, 512);
            layout->renderOrthographicProjection(patternPose, array);

            // Detecting and estimating the pose of the pattern
            PatternDetector* detector;
            detector = new PeriodicPatternDetector(15);
            detector->compute(array);
            Pose estimatedPose = detector->get2DPose();

            // Printing results 
            cout << "Estimation of pattern pose: " << estimatedPose.toString() << endl;
            
            UNIT_TEST(areEqual(patternPose.x, estimatedPose.x, 0.001))
            UNIT_TEST(areEqual(patternPose.y, estimatedPose.y, 0.001))
            UNIT_TEST(areEqual(patternPose.alpha, estimatedPose.alpha, 0.001))
        }

        static void runAllTests() {
            test2d();
        }

        static double speed(unsigned long testCount) {

            return 0.0;
        }


    };
}

