/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTQRCODEPOSEESTIMATION_HPP
#define TESTQRCODEPOSEESTIMATION_HPP

#include "QRCodePoseEstimation.hpp"
#include "UnitTest.hpp"
#include <iomanip>

namespace Vernier {
	class TestQRCodePoseEstimation {
	public:

		static void main() {
			cv::Mat image = cv::imread("G:/Utilisateurs/antoine.andre/Nextcloud/set_donnees_images_mire/Qrcode_flip/im_200.png", 1);
			cv::erode(image, image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)), cv::Point(1, 1));
			//cv::erode(image, image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3), cv::Point(1, 1)), cv::Point(-1, -1), 3);

			cv::flip(image, image, 0);

			//cv::namedWindow("image flip", cv::WINDOW_FREERATIO);
			//cv::imshow("image flip", image);
			//cv::waitKey();

			QRCodePoseEstimation poses;
			poses.resize(800, 0, 37);

			poses.detector.markerDetector.lowCannyThreshold = 75; //100
			poses.detector.markerDetector.highCannyThreshold = 100; //250

			poses.compute(image);
			poses.drawPose(image);

			cv::namedWindow("canny image", cv::WINDOW_FREERATIO);
			cv::imshow("canny image", poses.detector.markerDetector.cannyImage);

			cv::namedWindow("QR codes", cv::WINDOW_FREERATIO);
			cv::imshow("QR codes", image);

			poses.drawSnapshot();

			std::cout << poses.codes.size() << std::endl;

			for (std::map<int, Pose>::iterator it = poses.codes.begin(); it != poses.codes.end(); it++) {
				std::cout << "QR code num " << it->first << " at " << it->second.toString() << std::endl;
			}

			cv::waitKey();
		}

		static void mainNoNumber() {

			//cv::Mat oneSub(image.rows, image.cols, CV_8UC3);
			//oneSub.setTo(cv::Scalar(255,255,255));
			//image = oneSub - image;
			//cv::Mat image(image12bits.rows, image12bits.cols, CV_8U);
			//cv::cvtColor(image12bits, image, CV_8U);

			//image = image12bits;

			QRCodePoseEstimation pose;
			pose.resize(900, 0, 33);
			pose.detector.markerDetector.lowCannyThreshold = 75; //100
			pose.detector.markerDetector.highCannyThreshold = 80; //250

			Eigen::ArrayXXd analyseCPP(1, 3);

			for (int i = 0; i < 400; i++) {
				cv::Mat image = cv::imread("G:/Utilisateurs/antoine.andre/Documents/Essais_experimentaux/essaisSalleOptique/QRCodes/translation/29_01_20_50nanos_midi/essai_QRCode_translation_50nanos_29_01_20_midi_num_" + std::to_string(i) + ".tiff", cv::IMREAD_ANYDEPTH);
				cv::erode(image, image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)), cv::Point(1, 1));

				int raw_x_center = 1065;
				int raw_y_center = 959;

				image = image(cv::Rect(raw_x_center - 256, raw_y_center - 256, 512, 512));
				image(cv::Rect(400, 400, 112, 112)).setTo(1000);

				cv::normalize(image, image, 255, 0, cv::NORM_MINMAX);
				image.convertTo(image, CV_8UC1);
				pose.compute(image);
				pose.drawPose(image);
				cv::imshow("image", image);

				cv::waitKey(1);

				std::cout << "frame " << i << std::endl;
				std::cout << pose.codes[0].toString() << std::endl;

				analyseCPP.conservativeResize(analyseCPP.rows() + 1, 3);
				analyseCPP(analyseCPP.rows() - 2, 0) = pose.codes[0].x;
				analyseCPP(analyseCPP.rows() - 2, 1) = pose.codes[0].y;
			}

			std::ofstream file;
			file.open("G:/Utilisateurs/antoine.andre/Documents/Essais_experimentaux/essaisSalleOptique/QRCodes/translation/29_01_20_50nanos_midi/essai_QRCode_translation_50nanos_29_01_20_midi_num_analyse_py_canny.csv");
			file << "x,y\n";
			for (int i = 0; i < analyseCPP.rows(); i++) {
				for (int j = 0; j < analyseCPP.cols(); j++) {
					if (j == analyseCPP.cols() - 1) {
						file << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << analyseCPP(i, j);
					}
					else {
						file << analyseCPP(i, j) << ",";
					}
				}
				file << "\n";
			}
			file.close();
		}

		static void main2() {
			cv::Mat image = cv::imread("G:/Utilisateurs/antoine.andre/Desktop/QRCodeFiltered.png", -1);

			QRCodePoseEstimation poses = QRCodePoseEstimation(1, 250, 37);
			poses.resize(250, 1, 37);

			poses.detector.markerDetector.lowCannyThreshold = 100;
			poses.detector.markerDetector.highCannyThreshold = 253;

			poses.compute(image);
			poses.drawPose(image);

			cv::Mat canny;
			cv::namedWindow("canny image", cv::WINDOW_FREERATIO);
			cv::imshow("canny image", poses.detector.markerDetector.cannyImage);

			cv::namedWindow("QR codes", cv::WINDOW_FREERATIO);
			cv::imshow("QR codes", image);

			cv::waitKey();

			std::cout << "First way to get information from QR codes : " << std::endl;
			for (std::map<int, Pose>::iterator it = poses.codes.begin(); it != poses.codes.end(); it++) {
				std::cout << "QR code num " << it->first << " at " << it->second.toString() << std::endl;
			}
		}

		static void runAllTests() {
		}

		static double speed(unsigned long testCount) {
			cv::Mat image = cv::imread("QRCode/code31.jpg", -1);

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
}

#endif