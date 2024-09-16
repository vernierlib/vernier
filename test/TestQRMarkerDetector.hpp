/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTQRMARKERDETECTOR_HPP
#define TESTQRMARKERDETECTOR_HPP

#include "QRMarkerDetector.hpp"
#include "UnitTest.hpp"

namespace Vernier {
	class TestQRMarkerDetector {
	public:

		static void main() {
#ifdef USE_OPENCV
			cv::Mat image = cv::imread("QRCode/code21.jpg");

			QRMarkerDetector markerDetector;

			markerDetector.lowCannyThreshold = 200;
			markerDetector.highCannyThreshold = 400;

			markerDetector.compute(image);
			markerDetector.drawRowPatterns(image);
			markerDetector.drawColPatterns(image);
			markerDetector.drawGroupsOfPatterns(image);
			markerDetector.drawMarkers(image);
			std::cout << markerDetector.toString() << std::endl;

			cv::imshow("Edges", markerDetector.cannyImage);
			cv::imshow("Image", image);
			cv::waitKey(0);
#endif //USE_OPENCV

		}

		static void runAllTests() {
		}

		static double speed(unsigned long testCount) {
#ifdef USE_OPENCV
			QRMarkerDetector markerDetector;
			cv::Mat image;
			image = cv::imread("QRCode/code11.jpg");

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				markerDetector.compute(image);
			}
#endif //USE_OPENCV
			return toc(testCount);
		}
	};
}

#endif