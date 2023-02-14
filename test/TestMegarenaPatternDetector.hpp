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

    class TestMegarenaPatternDetector {
    public:

        static void main2d() {
//            // Loading the layout
//            string filename = "megarenaPattern.json";
//            PatternLayout* layout = Layout::loadFromJSON(filename);
//
//            // Setting the pose of the pattern in the camera frame for rendering
//            double x = 1500.0;
//            double y = 2000.0;
//            double rz = -0.5;
//            Eigen::Matrix4d patternPose = pose2d(x, y, rz).inverse();
//
//            // Allocating an array for rendering
//            Eigen::ArrayXXd array(1024, 1024);
//
//            // Rendering
//            double pixelSize = 1.0;
//            PatternRenderer patternRenderer(layout, pixelSize);
//            patternRenderer.renderOrthographicProjection(patternPose, array);
//
//            // Dectecting and estimating the pose of the pattern
//            PatternDetector* detector;
//            detector = Detector::loadFromJSON(filename);
//            detector->compute(array);
//
//            // Printing results 
//            cout << "Pattern pose in camera frame for rendering : " << endl << patternPose << endl;
//            cout << "------------------------------------------------------------------" << endl;
//            cout << "Estimation of pattern pose in camera frame : " << endl << detector->getPatternPose() << endl;
//            cout << "------------------------------------------------------------------" << endl;
//            cout << "Estimation of pattern pose in camera frame : " << endl << detector->getCameraPoseInPattern().getCameraPatternTransformation() << endl;
//            cout << "------------------------------------------------------------------" << endl;
//            cout << "Estimation of pattern pose in camera frame : " << endl << detector->getPatternPoseInCamera().getCameraPatternTransformation() << endl;
//
//            // Converting the array in opencv mat and showing it
//            Mat image(array.cols(), array.rows(), CV_64F, array.data());
//            transpose(image, image);
//            drawCameraFrame(image);
//            imshow(layout->getDescription() + " (" + filename + ")", image);
//            imshow("Spectrum", detector->getGuiSpectrum());
//            waitKey();
        }

        static void main3d() {
//            // Loading the layout
//            string filename = "megarenaPattern.json";
//            PatternLayout* layout = Layout::loadFromJSON(filename);
//
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

        static void runAllTests() {
        }

        static double speed(unsigned long testCount) {

            return 0.0;
        }


    };
}

