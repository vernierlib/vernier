/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "Thumbnail.hpp"
#include "UnitTest.hpp"


#ifndef TESTHUMBNAIL_HPP
#define TESTTHUMBNAIL_HPP

namespace vernier {
	class TestThumbnail {
	public:
		static void main() {
			Eigen::MatioFile file("Megarena/TestFilesMatMegarena.mat");
			Eigen::Vector3d planeCoeff1(1, 1);
			Eigen::Vector3d planeCoeff2(1, 1);
			file.read_mat("planeCoefficients1", planeCoeff1);
			file.read_mat("planeCoefficients2", planeCoeff2);

			Plane plane1(planeCoeff1);
			Plane plane2(planeCoeff2);
			Thumbnail thumbnail;

			int length1 = thumbnail.getLength(plane1, 1024, 1024);
			int length2 = thumbnail.getLength(plane2, 1024, 1024);

			std::cout << "size of thumbnail :  " << length1 << " x " << length2 << std::endl;
		}

		static void main2() {
			Eigen::MatioFile file("Megarena/TestFilesMatMegarena.mat");
			Eigen::Vector3d planeCoeff1(1, 1);
			Eigen::Vector3d planeCoeff2(1, 1);
			file.read_mat("planeCoefficients1", planeCoeff1);
			file.read_mat("planeCoefficients2", planeCoeff2);

			Eigen::ArrayXXcd patternArray;
			cv::Mat image = cv::imread("Megarena/coin_25_02_5.png");
			Eigen::MatrixXd intermediaryMatrix;
			cv::cv2eigen(image, intermediaryMatrix);
			patternArray = intermediaryMatrix.array();
			patternArray = 255 - patternArray;

			Plane plane1(planeCoeff1);
			Plane plane2(planeCoeff2);

			//std::cout << "plane 1 coeffs : " << plane1.getA() << "x + " << plane1.getB() << "y + " << plane1.getC() << std::endl;
			//std::cout << "plane 2 coeffs : " << plane2.getA() << "x + " << plane2.getB() << "y + " << plane2.getC() << std::endl;

			Thumbnail thumbnail;

			thumbnail.resize(thumbnail.getLength(plane1, 1024, 1024), thumbnail.getLength(plane2, 1024, 1024));
			thumbnail.compute(plane1, plane2, patternArray);
			thumbnail.getCodeSequence();
			cv::waitKey();
		}

		static double speed(unsigned long testCount) {
			Eigen::MatioFile file("Megarena/TestFilesMatMegarena.mat");
			Eigen::Vector3d planeCoeff1(1, 1);
			Eigen::Vector3d planeCoeff2(1, 1);
			file.read_mat("planeCoefficients1", planeCoeff1);
			file.read_mat("planeCoefficients2", planeCoeff2);

			Eigen::ArrayXXcd patternArray;
			cv::Mat image = cv::imread("Megarena/coin_25_02_5.png");

			cv::Mat gray;

			if (image.channels() > 1) {
				cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
			}
			else {
				gray = image;
			}

			gray.convertTo(gray, CV_8UC1);

			Eigen::MatrixXd intermediaryMatrix;
			cv::cv2eigen(gray, intermediaryMatrix);
			patternArray.real() = intermediaryMatrix.array();
			patternArray = 255 - patternArray;

			Plane plane1(planeCoeff1);
			Plane plane2(planeCoeff2);

			Thumbnail thumbnail;
			thumbnail.resize(thumbnail.getLength(plane1, 1024, 1024), thumbnail.getLength(plane2, 1024, 1024));

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				thumbnail.compute(plane1, plane2, patternArray);
			}

			return toc(testCount);
		}

		static double speedResize(unsigned long testCount) {
			Eigen::MatioFile file("Megarena/TestFilesMatMegarena.mat");
			Eigen::Vector3d planeCoeff1(1, 1);
			Eigen::Vector3d planeCoeff2(1, 1);
			file.read_mat("planeCoefficients1", planeCoeff1);
			file.read_mat("planeCoefficients2", planeCoeff2);

			Eigen::ArrayXXcd patternArray;
			cv::Mat image = cv::imread("Megarena/coin_25_02_5.png");

			cv::Mat gray;

			if (image.channels() > 1) {
				cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
			}
			else {
				gray = image;
			}

			gray.convertTo(gray, CV_8UC1);

			Eigen::MatrixXd intermediaryMatrix;
			cv::cv2eigen(gray, intermediaryMatrix);
			patternArray.real() = intermediaryMatrix.array();
			patternArray = 255 - patternArray;

			Plane plane1(planeCoeff1);
			Plane plane2(planeCoeff2);

			Thumbnail thumbnail;

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				thumbnail.resize(thumbnail.getLength(plane1, 1024, 1024), thumbnail.getLength(plane2, 1024, 1024));
			}

			return toc(testCount);
		}

		static double speedGetCode(unsigned long testCount) {
			Eigen::MatioFile file("Megarena/TestFilesMatMegarena.mat");
			Eigen::Vector3d planeCoeff1(1, 1);
			Eigen::Vector3d planeCoeff2(1, 1);
			file.read_mat("planeCoefficients1", planeCoeff1);
			file.read_mat("planeCoefficients2", planeCoeff2);

			Eigen::ArrayXXcd patternArray;
			cv::Mat image = cv::imread("Megarena/coin_25_02_5.png");

			cv::Mat gray;

			if (image.channels() > 1) {
				cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
			}
			else {
				gray = image;
			}

			gray.convertTo(gray, CV_8UC1);

			Eigen::MatrixXd intermediaryMatrix;
			cv::cv2eigen(gray, intermediaryMatrix);
			patternArray.real() = intermediaryMatrix.array();
			patternArray = 255 - patternArray;

			Plane plane1(planeCoeff1);
			Plane plane2(planeCoeff2);

			Thumbnail thumbnail;
			thumbnail.resize(thumbnail.getLength(plane1, 1024, 1024), thumbnail.getLength(plane2, 1024, 1024));
			thumbnail.compute(plane1, plane2, patternArray);

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				thumbnail.getCodeSequence();
			}

			return toc(testCount);
		}
	};
}
#endif // !TESTHUMBNAIL_HPP