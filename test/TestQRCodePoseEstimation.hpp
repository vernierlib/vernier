/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTQRCODEPOSEESTIMATION_HPP
#define TESTQRCODEPOSEESTIMATION_HPP

#include "QRCodePoseEstimation.hpp"
#include "QRCodePatternLayout.hpp"
#include "UnitTest.hpp"
#include <iomanip>

using namespace Vernier;
using namespace std;
using namespace cv;

class TestQRCodePoseEstimation {
public:

    static void main() {

        Mat image = cv::imread("testfiles/QRCode/code31.jpg");

        QRCodePoseEstimation estimator(15.5, 256, 37);

        estimator.detector.markerDetector.lowCannyThreshold = 50;
        estimator.detector.markerDetector.highCannyThreshold = 100;

        estimator.compute(image);
        estimator.drawPose(image);

        namedWindow("canny image", WINDOW_FREERATIO);
        imshow("canny image", estimator.detector.markerDetector.cannyImage);

        namedWindow("QR codes", WINDOW_FREERATIO);
        estimator.drawPose(image);
        imshow("QR codes", image);


        cout << "Found " << estimator.codes.size() << " markers" << endl;

        for (map<int, Pose>::iterator it = estimator.codes.begin(); it != estimator.codes.end(); it++) {
            cout << "QR code " << it->first << " at " << it->second.toString() << endl;
        }

        waitKey();
    }

    static void main2() {

        ofstream file("testfiles/Image140/results.csv");
        file << "no;x;y;alpha;pixelSize;no;x;y;alpha;pixelSize;" << endl;

        for (int i = 96; i <= 140; i++) {

            cout << "Loading image " << i << endl;
            Mat image = cv::imread("testfiles/Image140/Image" + to_string(i) + ".png");
            cv::Rect myROI(500, 500, 1500, 1000);
            image = image(myROI);
            bitwise_not(image, image);

            QRCodePoseEstimation estimator(15.5, 512, 33);
            estimator.detector.markerDetector.lowCannyThreshold = 200;
            estimator.detector.markerDetector.highCannyThreshold = 400;
            estimator.compute(image);
            //imshow("canny image", estimator.detector.markerDetector.cannyImage);

            estimator.drawPose(image);
            imshow("QR codes", image);
            moveWindow("QR codes", 512, 0);
            estimator.showControlImages();

            cout << "Found " << estimator.codes.size() << " markers" << endl;

            for (map<int, Pose>::iterator it = estimator.codes.begin(); it != estimator.codes.end(); it++) {
                cout << "QR code " << it->first << " at " << it->second.toString() << endl;
                file << it->first << ";" << it->second.x << ";" << it->second.y << ";" << it->second.alpha << ";" << it->second.pixelSize << ";";
            }
            file << endl;

            waitKey(1);
        }
        file.close();
    }

    static void mainNoNumber() {

        //Mat oneSub(image.rows, image.cols, CV_8UC3);
        //oneSub.setTo(Scalar(255,255,255));
        //image = oneSub - image;
        //Mat image(image12bits.rows, image12bits.cols, CV_8U);
        //cvtColor(image12bits, image, CV_8U);

        //image = image12bits;

        QRCodePoseEstimation pose;
        pose.resize(900, 0, 33);
        pose.detector.markerDetector.lowCannyThreshold = 75; //100
        pose.detector.markerDetector.highCannyThreshold = 80; //250

        Eigen::ArrayXXd analyseCPP(1, 3);

        for (int i = 0; i < 400; i++) {
            Mat image = imread("G:/Utilisateurs/antoine.andre/Documents/Essais_experimentaux/essaisSalleOptique/QRCodes/translation/29_01_20_50nanos_midi/essai_QRCode_translation_50nanos_29_01_20_midi_num_" + to_string(i) + ".tiff", IMREAD_ANYDEPTH);
            erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)), Point(1, 1));

            int raw_x_center = 1065;
            int raw_y_center = 959;

            image = image(Rect(raw_x_center - 256, raw_y_center - 256, 512, 512));
            image(Rect(400, 400, 112, 112)).setTo(1000);

            normalize(image, image, 255, 0, NORM_MINMAX);
            image.convertTo(image, CV_8UC1);
            pose.compute(image);
            pose.drawPose(image);
            imshow("image", image);

            waitKey(1);

            cout << "frame " << i << endl;
            cout << pose.codes[0].toString() << endl;

            analyseCPP.conservativeResize(analyseCPP.rows() + 1, 3);
            analyseCPP(analyseCPP.rows() - 2, 0) = pose.codes[0].x;
            analyseCPP(analyseCPP.rows() - 2, 1) = pose.codes[0].y;
        }

        ofstream file;
        file.open("G:/Utilisateurs/antoine.andre/Documents/Essais_experimentaux/essaisSalleOptique/QRCodes/translation/29_01_20_50nanos_midi/essai_QRCode_translation_50nanos_29_01_20_midi_num_analyse_py_canny.csv");
        file << "x,y\n";
        for (int i = 0; i < analyseCPP.rows(); i++) {
            for (int j = 0; j < analyseCPP.cols(); j++) {
                if (j == analyseCPP.cols() - 1) {
                    file << setprecision(numeric_limits<long double>::digits10 + 1) << analyseCPP(i, j);
                } else {
                    file << analyseCPP(i, j) << ",";
                }
            }
            file << "\n";
        }
        file.close();
    }

    static void main3() {
        Mat image = imread("G:/Utilisateurs/antoine.andre/Desktop/QRCodeFiltered.png", -1);

        QRCodePoseEstimation poses = QRCodePoseEstimation(1, 250, 37);
        poses.resize(250, 1, 37);

        poses.detector.markerDetector.lowCannyThreshold = 100;
        poses.detector.markerDetector.highCannyThreshold = 253;

        poses.compute(image);
        poses.drawPose(image);

        Mat canny;
        namedWindow("canny image", WINDOW_FREERATIO);
        imshow("canny image", poses.detector.markerDetector.cannyImage);

        namedWindow("QR codes", WINDOW_FREERATIO);
        imshow("QR codes", image);

        waitKey();

        cout << "First way to get information from QR codes : " << endl;
        for (map<int, Pose>::iterator it = poses.codes.begin(); it != poses.codes.end(); it++) {
            cout << "QR code num " << it->first << " at " << it->second.toString() << endl;
        }
    }

    static void test2d(int codeSize) {

        // Constructing the layout
        double physicalPeriod = 7.0;
        PatternLayout* layout = new QRCodePatternLayout(physicalPeriod, 1 + (codeSize - 1) / 2);
        cout << "  Code size: " << codeSize << endl;
        cout << "  Physical period: " << physicalPeriod << endl;

        // Setting the pose of the pattern in the camera frame for rendering
        double x = randomDouble(-50, 50);
        double y = randomDouble(-50, 50);
        double alpha = randomDouble(-PI, PI);
        double pixelSize = 0.9;
        Pose patternPose = Pose(x, y, alpha, pixelSize);
        cout << "  Pattern pose: " << patternPose.toString() << endl;

        // Rendering
        Eigen::ArrayXXd array(512, 512);
        layout->renderOrthographicProjection(patternPose, array);
        cv::Mat image = array2image(array);
        //imshow("QR codes", image);
        //waitKey();

        // Detecting
        QRCodePoseEstimation estimator(physicalPeriod, 256, codeSize);
        //estimator.detector.markerDetector.lowCannyThreshold = 100;
        //estimator.detector.markerDetector.highCannyThreshold = 200;
        //estimator.patternPhase.setSigma(1);
        estimator.compute(image);

        for (map<int, Pose>::iterator it = estimator.codes.begin(); it != estimator.codes.end(); it++) {
            cout << "  Marker " << it->first << " pose:" << it->second.toString() << endl;
            //it->second.draw(image);
        }
        // Drawing
        //estimator.showControlImages();
        //drawCameraFrame(image);
        //estimator.drawPose(image);

        //imshow("QR codes", image);
        //moveWindow("QR codes", 256, 0);

        //waitKey(1);

        if (estimator.codes.size() == 1) {
            Pose estimatedPose = estimator.codes.begin()->second;
            UNIT_TEST(areEqual(patternPose, estimatedPose, 0.5));
        }

    }

    static void test(string filename, int lowCannyThreshold, int highCannyThreshold, int numberOfMarkers, int snapshotSize, int numberOfHalfPeriod) {
#ifdef USE_OPENCV
        START_UNIT_TEST;
        cv::Mat image = imread(filename);

        QRCodePoseEstimation estimator = QRCodePoseEstimation(15.5, snapshotSize, numberOfHalfPeriod);
        estimator.detector.markerDetector.lowCannyThreshold = lowCannyThreshold;
        estimator.detector.markerDetector.highCannyThreshold = highCannyThreshold;

        estimator.compute(image);
        cout << "Found " << estimator.codes.size() << " markers" << endl;
        for (map<int, Pose>::iterator it = estimator.codes.begin(); it != estimator.codes.end(); it++) {
            cout << "QR code " << it->first << " at " << it->second.toString() << endl;
        }

        UNIT_TEST(estimator.codes.size() == numberOfMarkers);
#endif //USE_OPENCV            
    }

    static void runAllTests() {
        test("testfiles/QRCode/code17.jpg", 100, 200, 1, 512, 37);
        test("testfiles/QRCode/code23.png", 200, 400, 2, 512, 33);
        test("testfiles/QRCode/code31.jpg", 50, 100, 3, 256, 37);
        test("testfiles/QRCode/code61.jpg", 100, 210, 6, 256, 37);
        REPEAT_TEST(test2d(33), 10)
        REPEAT_TEST(test2d(37), 10)

    }

    static double speed(unsigned long testCount) {
        Mat image = imread("QRCode/code31.jpg", -1);

        QRCodePoseEstimation poses = QRCodePoseEstimation(1, 512, 37);
        poses.resize(256, 1, 37);
        poses.detector.markerDetector.lowCannyThreshold = 75;
        poses.detector.markerDetector.highCannyThreshold = 100;

        tic();
        for (unsigned long i = 0; i < testCount; i++) {
            poses.compute(image);
        }

        return toc(testCount);
    }

};


#endif