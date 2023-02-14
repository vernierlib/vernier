/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTPATTERNDETECTOR_HPP
#define TESTPATTERNDETECTOR_HPP

#include "QRCodePatternCannyDetector.hpp"
#include "PatternRenderer.hpp"

#include "Detector.hpp"
#include "Layout.hpp"

#include <iomanip>
#include <random>

namespace Vernier {
	class TestPatternDetector {
	public:

		static void mainPeriodicDetector() {
			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testPeriodicPattern.json");

			cv::Mat patternImg = cv::imread("Megarena/rotationSimu.png");
			mDetect->compute(patternImg);
			std::cout << "Periodic pattern detector poses :" << std::endl;
			std::cout << "Camera pose in Pattern" << mDetect->getCameraPoseInPattern().toString() << std::endl;
			std::cout << "Pattern pose in Camera" << mDetect->getPatternPoseInCamera().toString() << std::endl;
		}

		static void mainBitmapDetector() {
			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("BitmapPatternDetector.json");

			cv::Mat patternImg = cv::imread("img1.png");
			mDetect->compute(patternImg);

			std::cout << "Bitmap pattern detector poses :" << std::endl;
			std::cout << "Camera pose in Pattern" << mDetect->getCameraPoseInPattern().toString() << std::endl;
			std::cout << "Pattern pose in Camera" << mDetect->getPatternPoseInCamera().toString() << std::endl;
		}

		static void mainQRCodeBitmapDetector() {
			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("QRCodePatternDetector.json");
			cv::Mat patternImg = cv::imread("QRCode/code61.jpg");
			patternImg = patternImg(cv::Rect(930, 0, 512, 512));
			mDetect->compute(patternImg);

			std::cout << "QRCode bitmap pattern detector poses :" << std::endl;
			std::cout << "Camera pose in Pattern" << mDetect->getCameraPoseInPattern().toString() << std::endl;
			std::cout << "Pattern pose in Camera" << mDetect->getPatternPoseInCamera().toString() << std::endl;
		}

		static void mainMegarenaDetector() {
			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testMegarenaPattern.json");
			cv::Mat patternImg = cv::imread("Megarena/rotationSimu.png");
			mDetect->compute(patternImg);

			std::cout << "Megarena pattern detector poses :" << std::endl;
			std::cout << "Camera pose in Pattern" << mDetect->getCameraPoseInPattern().toString() << std::endl;
			std::cout << "Pattern pose in Camera" << mDetect->getPatternPoseInCamera().toString() << std::endl;
		}

		static void mainTestDetectors() {
			mainPeriodicDetector();
			std::cout << "\n";
			mainBitmapDetector();
			std::cout << "\n";
			mainQRCodeBitmapDetector();
			std::cout << "\n";
			mainMegarenaDetector();
		}

		static void mainPeriodicFullStack() {
			std::cout << "PERIODIC PATTERN LAYOUT AND DETECTOR FULL TEST" << std::endl;
			std::cout << "JSON Periodic pattern file generation" << std::endl;
			PeriodicPatternLayout layout(10, 137, 137);

			layout.saveToJSON("testPeriodicPattern.json");
			std::cout << "done" << std::endl;

			std::cout << "Periodic pattern rendering" << std::endl;
			PatternLayout* patternLayout;
			patternLayout = Layout::loadFromJSON("testPeriodicPattern.json");

			PatternRenderer patternRenderer(patternLayout, 1, 1);

			Eigen::ArrayXXd array(1024, 1024);

			patternRenderer.renderOrthographicProjection(Pose(-10, -5, 1000, 0.23, 0.0, 0.0), array);
			std::cout << "done" << std::endl;

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testPeriodicPattern.json");

			mDetect->compute(image);
			Vernier::Pose patternPose = mDetect->getCameraPoseInPattern();
			std::cout << "Periodic pattern detector poses :" << std::endl;
			std::cout << "Camera pose in Pattern" << patternPose.toString() << std::endl;
			std::cout << "Pattern pose in Camera" << mDetect->getPatternPoseInCamera().toString() << std::endl;


			cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
			cv::imshow(patternLayout->getClassname(), image);
			cv::imwrite(patternLayout->getClassname() + "forDoc.png", image * 255);
			cv::waitKey();
		}

		static void mainBitmapFullStack() {
			std::cout << "BITMAP PATTERN LAYOUT AND DETECTOR FULL TEST" << std::endl;
			std::cout << "JSON Bitmap pattern file generation" << std::endl;
			BitmapPatternLayout layout;
			layout.loadFromPNG("TestBitmapPatternUnit.png", 9);

			layout.saveToJSON("testBitmapPattern.json");
			std::cout << "done" << std::endl;

			std::cout << "Periodic pattern rendering" << std::endl;
			PatternLayout* patternLayout;
			patternLayout = Layout::loadFromJSON("testBitmapPattern.json");

			PatternRenderer patternRenderer(patternLayout, 1, 1);

			Eigen::ArrayXXd array(1024, 1024);

			patternRenderer.renderOrthographicProjection(Pose(-10, -5, 1000, 0.23, 0.0, 0.0), array);
			std::cout << "done" << std::endl;

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testBitmapPattern.json");

			mDetect->compute(image);
			Vernier::Pose patternPose = mDetect->getCameraPoseInPattern();
			std::cout << "Periodic pattern detector poses :" << std::endl;
			std::cout << "Camera pose in Pattern" << patternPose.toString() << std::endl;
			std::cout << "Pattern pose in Camera" << mDetect->getPatternPoseInCamera().toString() << std::endl;


			cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
			cv::imshow(patternLayout->getClassname(), image);
			cv::waitKey();
		}

		static void mainBitmapQRCodeFullStack() {
			std::cout << "QRCODE PATTERN LAYOUT AND DETECTOR FULL TEST" << std::endl;
			std::cout << "JSON Bitmap QRCode pattern file generation" << std::endl;
			QRCodePatternLayout layout(10, 37);

			layout.saveToJSON("testBitmapQRCodePattern.json");
			std::cout << "done" << std::endl;

			std::cout << "Periodic pattern rendering" << std::endl;
			PatternLayout* patternLayout;
			patternLayout = Layout::loadFromJSON("testBitmapQRCodePattern.json");

			PatternRenderer patternRenderer(patternLayout, 1, 1);

			Eigen::ArrayXXd array(1024, 1024);

			patternRenderer.renderOrthographicProjection(Pose(-10, -5, 1000, 0.23, 0.0, 0.0), array);
			std::cout << "done" << std::endl;

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testBitmapQRCodePattern.json");

			mDetect->compute(image);
			Vernier::Pose patternPose = mDetect->getCameraPoseInPattern();
			std::cout << "Periodic pattern detector poses :" << std::endl;
			std::cout << "Camera pose in Pattern" << patternPose.toString() << std::endl;
			std::cout << "Pattern pose in Camera" << mDetect->getPatternPoseInCamera().toString() << std::endl;


			cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
			cv::imshow(patternLayout->getClassname(), image);
			cv::imwrite(patternLayout->getClassname() + "forDoc.png", image * 255);
			cv::waitKey(1000);
		}

		static void mainMegarenaFullStack() {
			std::cout << "MEGARENA PATTERN LAYOUT AND DETECTOR FULL TEST" << std::endl;
			std::cout << "JSON file generation" << std::endl;
			MegarenaPatternLayout layout(4.5, 12);

			layout.setRegionOfInterest(Rectangle(1000, 1000, 3000, 2000));
			layout.saveToJSON("testMegarenaPattern.json");
			std::cout << "done" << std::endl;

			std::cout << "Megarena pattern rendering" << std::endl;
			PatternLayout* patternLayout;
			patternLayout = Layout::loadFromJSON("testMegarenaPattern.json");

			PatternRenderer patternRenderer(patternLayout, 30 * 18. / 1024, 1);

			Eigen::ArrayXXd array(1024, 1024);

			patternRenderer.renderOrthographicProjection(Pose(-1100, -1100, 1000, 0.21, 0, 0.0), array);

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());
			cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
			cv::imshow(patternLayout->getClassname(), image);
			cv::imwrite(patternLayout->getClassname() + "forDoc.png", image * 255);
			cv::waitKey();
		}

		static void mainTestAllFullStack() {
			std::cout << "Running all full stack layout and detectors" << std::endl;
			std::cout << "___________________________________________\n" << std::endl;
			mainPeriodicFullStack();
			//std::cout << "\n";
			//mainBitmapFullStack();
			//std::cout << "\n";
			//mainBitmapQRCodeFullStack();
			std::cout << "\n";
			mainMegarenaFullStack();
			cv::waitKey();
		}

		static void mainBitmap() {

			int nRows, nCols;
			Eigen::ArrayXXd mPattern = readPGMData("patternImage_testThumbnail.pgm", nRows, nCols);

			BitmapPatternDetector bitmapDetector("PeriodicPattern13periodsBitmap.json", 28);
			bitmapDetector.compute((char*)mPattern.data(), nRows, nCols);

			Pose pose = bitmapDetector.getPatternPoseInCamera();
			Pose pose2 = bitmapDetector.getCameraPoseInPattern();

			std::cout << pose.toString() << std::endl;
			std::cout << pose2.toString() << std::endl;

		}

		static void mainBitmap2() {

			int nRows, nCols;
			cv::Mat image = cv::imread("D:/These/Percipio/testPercipioDetector/img0.png", -1);
			image.convertTo(image, CV_64FC1);

			BitmapPatternDetector bitmapDetector("PeriodicPattern13periodsBitmap.json", 28);
			bitmapDetector.compute((char*)image.data, image.rows, image.cols);

			Pose pose = bitmapDetector.getPatternPoseInCamera();
			Pose pose2 = bitmapDetector.getCameraPoseInPattern();

			std::cout << pose.toString() << std::endl;
			std::cout << pose2.toString() << std::endl;

		}

		static void mainQRCodeCanny() {
			cv::Mat image = cv::imread("QRCode/code31.jpg", -1);

			QRCodePatternCannyDetector qrCodeDetector(1, 256, 37, 75, 100);

			qrCodeDetector.compute(image);
		}

		static void runPeriodicTest(int sizeImg) {
			std::random_device rd;

			// Layout generation
			std::uniform_real_distribution<double> dist(4, 8);
			double period = dist(rd);

			PeriodicPatternLayout layout(period, 437, 437);

			layout.saveToJSON("testPeriodicPattern.json");

			// Pattern rendering
			PatternLayout* patternLayout;
			patternLayout = Layout::loadFromJSON("testPeriodicPattern.json");

			PatternRenderer patternRenderer(patternLayout, 1, 1);

			Eigen::ArrayXXd array(sizeImg, sizeImg);

			std::uniform_real_distribution<double> dist2(-0.9 * period, 0.9 * period);
			std::uniform_real_distribution<double> dist3(0, PI / 5.0);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);
			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			// Pattern pose computing
			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testPeriodicPattern.json");

			mDetect->compute(image);
			//cv::imwrite("periodicPatternImg.png", image * 255);
			Vernier::Pose patternPose = mDetect->getCameraPoseInPattern();

			//std::cout << "Sent pose periodic : " << Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0).toString() << std::endl;
			std::cout << xPosition + patternPose.x << "\t" << yPosition + patternPose.y << "\t" << alphaOrientation - patternPose.alpha << std::endl;
			UNIT_TEST(areEqual(-xPosition, patternPose.x, 1e-4) && areEqual(-yPosition, patternPose.y, 1e-4) && areEqual(alphaOrientation, patternPose.alpha, 1e-4));
		}

		static void runBitmapTest(int sizeImg) {
			std::random_device rd;

			// Layout generation
			std::uniform_real_distribution<double> dist(10, 15);
			double period = dist(rd);

			BitmapPatternLayout layout;
			layout.loadFromPNG("bitmapUnitTest.png", period);
			//layout.setApproxPxPeriod(period * 2.0);
			layout.saveToJSON("testBitmapPattern.json");

			PatternLayout* patternLayout;
			patternLayout = Layout::loadFromJSON("testBitmapPattern.json");

			PatternRenderer patternRenderer(patternLayout, 1, 1);


			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testBitmapPattern.json");

			Eigen::ArrayXXd array(sizeImg, sizeImg);
			std::uniform_real_distribution<double> dist2(-array.rows() / 8, array.rows() / 8);
			std::uniform_real_distribution<double> dist3(0.0, 2.0 * PI);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);
			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);

			double xCamera = cos(alphaOrientation) * xPosition - sin(alphaOrientation) * yPosition;
			double yCamera = sin(alphaOrientation) * xPosition + cos(alphaOrientation) * yPosition;

			//std::cout << "SENT POSE : " << array.rows() / 2.0 + xCamera << "\t" << array.cols() / 2.0 + yCamera << "\t" << alphaOrientation << std::endl;

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());


			mDetect->compute(image);
			//cv::imwrite("bitmapPatternImg.png", image * 255);
			Vernier::Pose patternPose = mDetect->getPatternPoseInCamera();
			//std::cout << "COMPUTED POSE : " << patternPose.x << "\t" << patternPose.y << "\t" << patternPose.alpha << std::endl;

			//std::cout << "ERROR : " << array.rows() / 2.0 + xCamera - patternPose.x << "\t" << array.cols() / 2.0 + yCamera - patternPose.y << "\t" << alphaOrientation - patternPose.alpha << "\n" << std::endl;

			UNIT_TEST(areEqual(array.rows() / 2.0 + xCamera, patternPose.x, 0.01) && areEqual(array.cols() / 2.0 + yCamera, patternPose.y, 0.01));

		}

		static void runQRCodeTest(int sizeImg) {
			std::random_device rd;

			// Layout generation
			std::uniform_real_distribution<double> dist(10, 15);
			double period = dist(rd);

			QRCodePatternLayout layout(period, 37);

			//layout.setApproxPxPeriod(period * 2.0);
			layout.saveToJSON("testQRCodePattern.json");

			PatternLayout* patternLayout;
			patternLayout = Layout::loadFromJSON("testQRCodePattern.json");

			PatternRenderer patternRenderer(patternLayout, 1, 1);


			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testQRCodePattern.json");

			Eigen::ArrayXXd array(sizeImg, sizeImg);
			std::uniform_real_distribution<double> dist2(-array.rows() / 8, array.rows() / 8);
			std::uniform_real_distribution<double> dist3(0.0, 2.0 * PI);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);
			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);

			double xCamera = cos(alphaOrientation) * xPosition - sin(alphaOrientation) * yPosition;
			double yCamera = sin(alphaOrientation) * xPosition + cos(alphaOrientation) * yPosition;

			//std::cout << "SENT POSE : " << array.rows() / 2.0 + xCamera << "\t" << array.cols() / 2.0 + yCamera << "\t" << alphaOrientation << std::endl;

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			cv::imshow("pattern", image);

			mDetect->compute(image);
			//cv::imwrite("qrCodePatternImg.png", image * 255);
			Vernier::Pose patternPose = mDetect->getPatternPoseInCamera();
			//std::cout << "COMPUTED POSE : " << patternPose.x << "\t" << patternPose.y << "\t" << patternPose.alpha << std::endl;

			//std::cout << "ERROR : " << array.rows() / 2.0 + xCamera - patternPose.x << "\t" << array.cols() / 2.0 + yCamera - patternPose.y << "\t" << alphaOrientation - patternPose.alpha << "\n" << std::endl;

			UNIT_TEST(areEqual(array.rows() / 2.0 + xCamera, patternPose.x, 0.01) && areEqual(array.cols() / 2.0 + yCamera, patternPose.y, 0.01));

			//cv::waitKey();
			//cv::destroyAllWindows();
		}


		static void runMegarenaTest(int sizeImg) {
			std::random_device rd;

			// Layout generation
			std::uniform_real_distribution<double> dist(5, 8);
			double period = dist(rd);

			MegarenaPatternLayout layout(period, 12);

			layout.setRegionOfInterest(Rectangle(1000, 1000, 3000, 2000));
			layout.saveToJSON("testMegarenaPattern.json");

			// Pattern rendering
			PatternLayout* patternLayout;
			patternLayout = Layout::loadFromJSON("testMegarenaPattern.json");

			PatternRenderer patternRenderer(patternLayout, 1, 1);

			Eigen::ArrayXXd array(sizeImg, sizeImg);


			std::uniform_real_distribution<double> dist2(-10000, -900);
			std::uniform_real_distribution<double> dist3(0, PI * 2.0);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);

			//std::cout << xPosition << "; " << yPosition << "; " << alphaOrientation << std::endl;

			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);
			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			// Pattern pose computing
			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testMegarenaPattern.json");

			mDetect->compute(image);
			//cv::imwrite("megarenaPatternImg.png", image * 255);
			Vernier::Pose patternPose = mDetect->getCameraPoseInPattern();
			//std::cout << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << "Camera pose in Pattern" << patternPose.toString() << std::endl;

			std::cout << "diff with pose : " << xPosition + patternPose.y << "; " << yPosition + patternPose.x << "; " << patternPose.alpha - alphaOrientation << "\n" << std::endl;
			UNIT_TEST(areEqual(xPosition, -patternPose.y, 0.001) && areEqual(yPosition, -patternPose.x, 0.001) && areEqual(alphaOrientation, patternPose.alpha, 0.001));
		}

		static double speedBitmap(unsigned long testCount, int sizeImg) {
			std::random_device rd;

			// Layout generation

			double minPeriod, maxPeriod;
			if (sizeImg <= 256) {
				minPeriod = 3;
				maxPeriod = 4.5;
			}
			else if (sizeImg > 256 && sizeImg <= 512) {
				minPeriod = 4;
				maxPeriod = 8;
			}
			else if (sizeImg > 512 && sizeImg <= 1024) {
				minPeriod = 10;
				maxPeriod = 15;
			}
			else {
				minPeriod = 15;
				maxPeriod = 20;
			}

			std::uniform_real_distribution<double> dist(minPeriod, maxPeriod);
			double period = dist(rd);

			BitmapPatternLayout layout;
			layout.loadFromPNG("bitmapUnitTest.png", period);

			//std::cout << "ok" << std::endl;

			layout.saveToJSON("testBitmapPattern.json");

			PatternRenderer patternRenderer(&layout, 1, 1);


			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testBitmapPattern.json");

			Eigen::ArrayXXd array(sizeImg, sizeImg);
			std::uniform_real_distribution<double> dist2(-array.rows() / 8, array.rows() / 8);
			std::uniform_real_distribution<double> dist3(0.0, 2.0 * PI);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);
			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());
			//cv::imshow("bitmap pattern", image);
			//cv::waitKey();

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				mDetect->compute(image);
			}

			delete mDetect;

			return toc(testCount);
		}

		static double speedBitmapSymetric(unsigned long testCount, int sizeImg) {
			std::random_device rd;

			// Layout generation

			double minPeriod, maxPeriod;
			if (sizeImg <= 256) {
				minPeriod = 4;
				maxPeriod = 5.5;
			}
			else if (sizeImg > 256 && sizeImg <= 512) {
				minPeriod = 6;
				maxPeriod = 10;
			}
			else if (sizeImg > 512 && sizeImg <= 1024) {
				minPeriod = 10;
				maxPeriod = 15;
			}
			else {
				minPeriod = 15;
				maxPeriod = 20;
			}

			std::uniform_real_distribution<double> dist(minPeriod, maxPeriod);
			double period = dist(rd);

			BitmapPatternLayout layout;
			layout.loadFromPNG("bitmapSymetricUnitTest.png", period);

			layout.saveToJSON("testBitmapPatternSymetric.json");

			PatternRenderer patternRenderer(&layout, 1, 1);


			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testBitmapPatternSymetric.json");

			Eigen::ArrayXXd array(sizeImg, sizeImg);
			std::uniform_real_distribution<double> dist2(-array.rows() / 8, array.rows() / 8);
			std::uniform_real_distribution<double> dist3(0.0, PI / 2.0);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);
			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				mDetect->compute(image);
			}

			delete mDetect;

			return toc(testCount);
		}

		static double speedQRCode(unsigned long testCount, int sizeImg) {
			std::random_device rd;

			// Layout generation

			double minPeriod, maxPeriod;
			if (sizeImg <= 256) {
				minPeriod = 4;
				maxPeriod = 5.5;
			}
			else if (sizeImg > 256 && sizeImg <= 512) {
				minPeriod = 6;
				maxPeriod = 10;
			}
			else if (sizeImg > 512 && sizeImg <= 1024) {
				minPeriod = 10;
				maxPeriod = 15;
			}
			else {
				minPeriod = 15;
				maxPeriod = 20;
			}

			std::uniform_real_distribution<double> dist(minPeriod, maxPeriod);
			double period = dist(rd);

			QRCodePatternLayout layout(period, 37);
			layout.saveToJSON("testQRCodePattern.json");

			PatternRenderer patternRenderer(&layout, 1, 1);


			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testQRCodePattern.json");

			Eigen::ArrayXXd array(sizeImg, sizeImg);
			std::uniform_real_distribution<double> dist2(-array.rows() / 8, array.rows() / 8);
			std::uniform_real_distribution<double> dist3(0.0, PI * 2.0);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);
			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				mDetect->compute(image);
			}

			delete mDetect;

			return toc(testCount);
		}

		static double speedPeriodic(unsigned long testCount, int sizeImg) {
			std::random_device rd;

			// Layout generation

			double minPeriod, maxPeriod;
			if (sizeImg <= 256) {
				minPeriod = 4;
				maxPeriod = 6;
			}
			else if (sizeImg > 256 && sizeImg <= 512) {
				minPeriod = 8;
				maxPeriod = 12;
			}
			else {
				minPeriod = 10;
				maxPeriod = 15;
			}

			std::uniform_real_distribution<double> dist(minPeriod, maxPeriod);
			double period = dist(rd);

			PeriodicPatternLayout layout(period, 437, 437);
			PatternRenderer patternRenderer(&layout, 1, 1);

			Eigen::ArrayXXd array(sizeImg, sizeImg);

			std::uniform_real_distribution<double> dist2(-0.9 * period, 0.9 * period);
			std::uniform_real_distribution<double> dist3(0, PI / 5.0);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);
			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);

			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			// Pattern pose computing

			PeriodicPatternDetector mDetect;

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				mDetect.compute(image);
			}

			return toc(testCount);
		}

		static double speedMegarena(unsigned long testCount, int sizeImg) {

			std::random_device rd;

			// Layout generation
			double minPeriod, maxPeriod;
			if (sizeImg <= 256) {
				minPeriod = 4;
				maxPeriod = 6;
			}
			else if (sizeImg > 256 && sizeImg <= 512) {
				minPeriod = 8;
				maxPeriod = 10;
			}
			else if (sizeImg > 512 && sizeImg <= 1024) {
				minPeriod = 10;
				maxPeriod = 15;
			}
			else {
				minPeriod = 20;
				maxPeriod = 30;
			}

			std::uniform_real_distribution<double> dist(minPeriod, maxPeriod);
			double period = dist(rd);

			MegarenaPatternLayout layout(period, 12);
			layout.saveToJSON("testMegarenaPattern.json");

			PatternRenderer patternRenderer(&layout, 1, 1);

			Eigen::ArrayXXd array(sizeImg, sizeImg);


			std::uniform_real_distribution<double> dist2(-5000, -900);
			std::uniform_real_distribution<double> dist3(0, PI * 2.0);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);

			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);
			cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());

			// Pattern pose computing
			PatternDetector* mDetect;
			mDetect = Detector::loadFromJSON("testMegarenaPattern.json");

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				mDetect->compute(image);
			}

			delete mDetect;

			return toc(testCount);
		}

		static double speedSimpleMegarena(unsigned long testCount) {

			std::random_device rd;
			std::uniform_real_distribution<double> dist(8, 15);
			double period = dist(rd);
			int size = 1024;

			MegarenaPatternLayout layout(period / 2.0, 12);
			PatternRenderer patternRenderer(&layout, 1, 1);

			Eigen::ArrayXXd array(size, size);
			Eigen::ArrayXXcd patternArray(size, size);


			std::uniform_real_distribution<double> dist2(-5000, -900);
			std::uniform_real_distribution<double> dist3(0, PI * 2.0);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);

			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);

			MegarenaDetector mDetect(12, size, size, period, period, 0.005);

			patternArray.real() = array;

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				mDetect.compute(patternArray);
			}

			return toc(testCount);

		}


		static double speedSimplePeriodic(unsigned long testCount) {

			std::random_device rd;
			std::uniform_real_distribution<double> dist(8, 15);
			double period = dist(rd);
			int size = 1024;

			PeriodicPatternLayout layout(period / 2.0, 437, 437);
			PatternRenderer patternRenderer(&layout, 1, 1);

			Eigen::ArrayXXd array(size, size);
			Eigen::ArrayXXcd patternArray(size, size);


			std::uniform_real_distribution<double> dist2(-period / 2.0, period / 2.0);
			std::uniform_real_distribution<double> dist3(0, PI * 2.0);

			double xPosition = dist2(rd);
			double yPosition = dist2(rd);
			double alphaOrientation = dist3(rd);

			patternRenderer.renderOrthographicProjection(Pose(xPosition, yPosition, 1000, alphaOrientation, 0.0, 0.0), array);

			PeriodicPatternDetector mDetect(period, period);

			patternArray.real() = array;

			tic();
			for (unsigned long i = 0; i < testCount; i++) {
				mDetect.compute(patternArray);
			}

			return toc(testCount);

		}

		static void runAllSpeedTests(int testCount, int numberTests) {
			double avg2048, avg1024, avg512, avg256;

			avg2048 = 0;
			avg1024 = 0;
			avg512 = 0;
			avg256 = 0;
			for (int i = 0; i < testCount; i++) {
				//avg2048 += speedQRCode(numberTests, 2048);
				avg1024 += speedQRCode(numberTests, 1024);
				avg512 += speedQRCode(numberTests, 512);
				//avg256 += speedQRCode(numberTests, 256);
			}

			std::cout << "QRCODE DETECTOR \n2048 : " << avg2048 / (double)testCount << " ms" << std::endl;
			std::cout << "1024 : " << avg1024 / (double)testCount << " ms" << std::endl;
			std::cout << "512 : " << avg512 / (double)testCount << " ms" << std::endl;
			std::cout << "256 : " << avg256 / (double)testCount << " ms\n" << std::endl;

			avg2048 = 0;
			avg1024 = 0;
			avg512 = 0;
			avg256 = 0;
			for (int i = 0; i < testCount; i++) {
				avg2048 += speedMegarena(numberTests, 2048);
				avg1024 += speedMegarena(numberTests, 1024);
				avg512 += speedMegarena(numberTests, 512);
				avg256 += speedMegarena(numberTests, 256);
			}

			std::cout << "MEGARENA DETECTOR \n2048 : " << avg2048 / (double)testCount << " ms" << std::endl;
			std::cout << "1024 : " << avg1024 / (double)testCount << " ms" << std::endl;
			std::cout << "512 : " << avg512 / (double)testCount << " ms" << std::endl;
			std::cout << "256 : " << avg256 / (double)testCount << " ms\n" << std::endl;
		}

	};
}

#endif // !TESTPATTERNDETECTOR_HPP