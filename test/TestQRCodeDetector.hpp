/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTQRCODEDETECTOR_HPP
#define TESTQRCODEDETECTOR_HPP

#include "QRCodeDetector.hpp"
#include "UnitTest.hpp"

namespace Vernier {
	class TestQRCodeDetector {
	public:

		static void main() {
			cv::Mat image = cv::imread("QRCode/code19.png");

			QRCodeDetector codeDetector;

			codeDetector.markerDetector.lowCannyThreshold = 20;
			codeDetector.markerDetector.highCannyThreshold = 100;

			codeDetector.compute(image);
			codeDetector.draw(image);
			std::cout << codeDetector.markerDetector.toString() << std::endl;
			std::cout << codeDetector.toString() << std::endl;

			cv::imshow("Edges", codeDetector.markerDetector.cannyImage);
			cv::imshow("Image", image);

			cv::waitKey(0);
		}

		static void runAllTests() {
			QRCodeDetector codeDetector;
			cv::Mat image;

			image = cv::imread("QRCode/code11.jpg");
			codeDetector.compute(image);
			UNIT_TEST(codeDetector.codes.size() == 1);

			image = cv::imread("QRCode/code12.jpg");
			codeDetector.compute(image);
			UNIT_TEST(codeDetector.codes.size() == 1);

			image = cv::imread("QRCode/code13.jpg");
			codeDetector.compute(image);
			UNIT_TEST(codeDetector.codes.size() == 1);

			image = cv::imread("QRCode/code14.jpg");
			codeDetector.compute(image);
			UNIT_TEST(codeDetector.codes.size() == 1);

			image = cv::imread("QRCode/code15.jpg");
			codeDetector.compute(image);
			UNIT_TEST(codeDetector.codes.size() == 1);

			image = cv::imread("QRCode/code16.jpg");
			codeDetector.markerDetector.lowCannyThreshold = 500;
			codeDetector.markerDetector.highCannyThreshold = 600;
			codeDetector.compute(image);
			UNIT_TEST(codeDetector.codes.size() == 1);

			image = cv::imread("QRCode/code17.jpg");
			codeDetector.markerDetector.lowCannyThreshold = 100;
			codeDetector.markerDetector.highCannyThreshold = 200;
			codeDetector.compute(image);
			UNIT_TEST(codeDetector.codes.size() == 1);

			image = cv::imread("QRCode/code18.jpg");
			cv::erode(image, image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)), cv::Point(-1, -1), 3);
			codeDetector.markerDetector.lowCannyThreshold = 50;
			codeDetector.markerDetector.highCannyThreshold = 100;
			UNIT_TEST(codeDetector.codes.size() == 1);

			image = cv::imread("QRCode/code31.jpg");
			codeDetector.markerDetector.lowCannyThreshold = 50;
			codeDetector.markerDetector.highCannyThreshold = 100;
			codeDetector.compute(image);
			UNIT_TEST(codeDetector.codes.size() == 3);

			image = cv::imread("QRCode/code61.jpg");
			codeDetector.markerDetector.lowCannyThreshold = 120;
			codeDetector.markerDetector.highCannyThreshold = 200;
			codeDetector.compute(image);
			UNIT_TEST(codeDetector.codes.size() == 6);
		}

		static double speed(unsigned long testCount) {
			QRCodeDetector codeDetector;
			cv::Mat image;
			image = cv::imread("code11.jpg");

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				codeDetector.compute(image);
			}
			return toc(testCount);
		}
	};
}

#endif