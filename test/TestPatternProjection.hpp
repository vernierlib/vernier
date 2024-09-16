/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTPATTERNPROJECTION_HPP
#define TESTPATTERNPROJECTION_HPP

//#include <opencv2/highgui.hpp>
#include "PatternProjection.hpp"
#include "UnitTest.hpp"
#include <random>

namespace Vernier {

    class TestPatternProjection {
    public:

        static void main() {
            cv::Mat intrisicMat(3, 3, CV_64F); // Intrisic matrix
            intrisicMat.at<double>(0, 0) = 2500.0;
            intrisicMat.at<double>(1, 0) = 0;
            intrisicMat.at<double>(2, 0) = 0;

            intrisicMat.at<double>(0, 1) = 0;
            intrisicMat.at<double>(1, 1) = 2500.0;
            intrisicMat.at<double>(2, 1) = 0;

            intrisicMat.at<double>(0, 2) = 320;
            intrisicMat.at<double>(1, 2) = 240;
            intrisicMat.at<double>(2, 2) = 1;

            cv::Mat rVec(3, 1, CV_64F); // Rotation vector
            rVec.at<double>(0) = 0;
            rVec.at<double>(1) = 0.0;
            rVec.at<double>(2) = 0.0; //PI/6;

            cv::Mat tVec(3, 1, CV_64F); // Translation vector
            tVec.at<double>(0) = 0.0;
            tVec.at<double>(1) = 0.0;
            tVec.at<double>(2) = 1000;

            cv::Mat distortionCoefficients(5, 1, CV_64F); // Distortion vector
            distortionCoefficients.at<double>(0) = 0.0;
            distortionCoefficients.at<double>(1) = 0.0;
            distortionCoefficients.at<double>(2) = 0.0;
            distortionCoefficients.at<double>(3) = 0.0;
            distortionCoefficients.at<double>(4) = 0.0;

            std::cout << "Intrisic matrix: " << intrisicMat << std::endl << std::endl;
            std::cout << "Rotation vector: " << rVec << std::endl << std::endl;
            std::cout << "Translation vector: " << tVec << std::endl << std::endl;
            std::cout << "Distortion coef: " << distortionCoefficients << std::endl << std::endl;

            double x = 1300;
            double y = 1500;

            Eigen::MatioFile file("Megarena/newMask12Bits_x3_2pi.mat", MAT_ACC_RDONLY);
            Eigen::ArrayXXi bitSequence(1, 1);
            file.read_mat("bit_sequence_codee", bitSequence);
            MegarenaPatternLayout pattern = MegarenaPatternLayout(4.5, bitSequence, cv::Rect_<double>(x - 500, y - 500, 1000, 1000));

            PatternProjection projection(intrisicMat, distortionCoefficients);
            projection.setPattern(&pattern, 0.8);
            cv::Mat image;
            image.create(480, 640, CV_8U);

            double theta = 0;
            rVec.at<double>(2) = theta;
            tVec.at<double>(0) = x;
            tVec.at<double>(1) = y;
            projection.getImage(tVec, rVec, image);
            cv::imshow("Pattern projection", image);
            //cv::imwrite("test" + toString(i) + ".png", image);
            cv::waitKey();

            //for (int i = 0; i <= 360; i++) {
            //	double theta = i * 2 * PI / 360.0;
            //	rVec.at<double>(2) = theta;
            //	tVec.at<double>(0) = -x * cos(theta) + y * sin(theta);
            //	tVec.at<double>(1) = -x * sin(theta) - y * cos(theta);
            //	projection.getImage(tVec, rVec, image);
            //	cv::imshow("Pattern projection", image);
            //	//cv::imwrite("test" + toString(i) + ".png", image);
            //	cv::waitKey(10);
            //}
        }

        static void main2() {
            cv::Mat intrisicMat(3, 3, CV_64F); // Intrisic matrix
            intrisicMat.at<double>(0, 0) = 0.0335;
            intrisicMat.at<double>(1, 0) = 0;
            intrisicMat.at<double>(2, 0) = 0;

            intrisicMat.at<double>(0, 1) = 0;
            intrisicMat.at<double>(1, 1) = 0.0335;
            intrisicMat.at<double>(2, 1) = 0;

            intrisicMat.at<double>(0, 2) = 256;
            intrisicMat.at<double>(1, 2) = 256;
            intrisicMat.at<double>(2, 2) = 1;

            cv::Mat rVec(3, 1, CV_64F); // Rotation vector
            rVec.at<double>(0) = 0.0;
            rVec.at<double>(1) = 0.0;
            rVec.at<double>(2) = 0.0; //PI/6;

            cv::Mat tVec(3, 1, CV_64F); // Translation vector
            tVec.at<double>(0) = 0.0;
            tVec.at<double>(1) = 0.0;
            tVec.at<double>(2) = 0.015;

            cv::Mat distortionCoefficients(5, 1, CV_64F); // Distortion vector
            distortionCoefficients.at<double>(0) = 0.0;
            distortionCoefficients.at<double>(1) = 0.0;
            distortionCoefficients.at<double>(2) = 0.0;
            distortionCoefficients.at<double>(3) = 0.0;
            distortionCoefficients.at<double>(4) = 0.0;

            std::cout << "Intrisic matrix: " << intrisicMat << std::endl << std::endl;
            std::cout << "Rotation vector: " << rVec << std::endl << std::endl;
            std::cout << "Translation vector: " << tVec << std::endl << std::endl;
            std::cout << "Distortion coef: " << distortionCoefficients << std::endl << std::endl;

            // random position
            double min = 0;
            double max = 10;
            std::random_device rd; // only used once to initialise (seed) engine
            std::mt19937 rng(rd()); // random-number engine used (Mersenne-Twister in this case)
            std::uniform_int_distribution<int> uni1(min, max); // guaranteed unbiased

            auto random_integerX = uni1(rng);

            std::uniform_int_distribution<int> uni2(min, max); // guaranteed unbiased

            auto random_integerY = uni2(rng);

            double x = 2500;
            double y = 4300;

            double theta = 30 * 2 * PI / 360.0;
            rVec.at<double>(2) = theta;

            std::cout << "random X : " << x << std::endl;
            std::cout << "random Y : " << y << std::endl;

            Eigen::MatioFile file("Megarena/newMask12Bits_x3_2pi.mat", MAT_ACC_RDONLY);
            Eigen::ArrayXXi bitSequence(1, 1);
            file.read_mat("bit_sequence_codee", bitSequence);

            //PeriodicPattern pattern = PeriodicPattern(4, 501, 501);

            MegarenaPatternLayout pattern = MegarenaPatternLayout(3.0, bitSequence, cv::Rect_<double>(x - 1500, y - 1500, 3000, 3000));

            PatternProjection projection(intrisicMat, distortionCoefficients);
            projection.setPattern(&pattern, 0.5);

            cv::Mat image;
            image.create(512, 512, CV_8U);
            for (int i = 0; i <= 500; i++) {
                tVec.at<double>(0) = -(x) * cos(theta) + (y) * sin(theta) - i * random_integerX;
                tVec.at<double>(1) = -(x) * sin(theta) - (y) * cos(theta) - i * random_integerY;

                projection.getImage(tVec, rVec, image);
                //cv::line(image, cv::Point(512, 0), cv::Point(512, 768), (255, 255, 255));
                //cv::line(image, cv::Point(0, 384), cv::Point(1024, 384), (255, 255, 255));

                //cv::line(image, cv::Point(900-20, 700), cv::Point(900+20, 700), (255, 255, 255));
                //cv::line(image, cv::Point(900, 700-20), cv::Point(900, 700+20), (255, 255, 255));
                //cv::GaussianBlur(image, image, cv::Size(5, 5), 0, 0);
                cv::imshow("Pattern projection", image);
                //cv::imwrite("F:/These/PortageCppMires/fullProject/fullProject/Datas/translation_simu_10_05_19/translation_10_05_19_num_" + toString(i) + ".png", image);
                cv::waitKey(10);
            }
        }

        static void main3() {
            cv::Mat intrisicMat(3, 3, CV_64F); // Intrisic matrix
            intrisicMat.at<double>(0, 0) = 2000.0;
            intrisicMat.at<double>(1, 0) = 0;
            intrisicMat.at<double>(2, 0) = 0;

            intrisicMat.at<double>(0, 1) = 0;
            intrisicMat.at<double>(1, 1) = 2000.0;
            intrisicMat.at<double>(2, 1) = 0;

            intrisicMat.at<double>(0, 2) = 200;
            intrisicMat.at<double>(1, 2) = 200;
            intrisicMat.at<double>(2, 2) = 1;

            cv::Mat rVec(3, 1, CV_64F); // Rotation vector
            rVec.at<double>(0) = 0.0;
            rVec.at<double>(1) = 0.0;
            rVec.at<double>(2) = 0.0; //PI/6;

            cv::Mat tVec(3, 1, CV_64F); // Translation vector
            tVec.at<double>(0) = 0.0;
            tVec.at<double>(1) = 0.0;
            tVec.at<double>(2) = 1000;

            cv::Mat distortionCoefficients(5, 1, CV_64F); // Distortion vector
            distortionCoefficients.at<double>(0) = 0.0;
            distortionCoefficients.at<double>(1) = 0.0;
            distortionCoefficients.at<double>(2) = 0.0;
            distortionCoefficients.at<double>(3) = 0.0;
            distortionCoefficients.at<double>(4) = 0.0;

            std::cout << "Intrisic matrix: " << intrisicMat << std::endl << std::endl;
            std::cout << "Rotation vector: " << rVec << std::endl << std::endl;
            std::cout << "Translation vector: " << tVec << std::endl << std::endl;
            std::cout << "Distortion coef: " << distortionCoefficients << std::endl << std::endl;

            double x = 1500;
            double y = 1500;

            Eigen::MatioFile file("Megarena/newMask12Bits_x3_2pi.mat", MAT_ACC_RDONLY);
            Eigen::ArrayXXi bitSequence(1, 1);
            file.read_mat("bit_sequence_codee", bitSequence);

            //PeriodicPattern pattern = PeriodicPattern(4, 501, 501);
            MegarenaPatternLayout pattern = MegarenaPatternLayout(3.0, bitSequence, cv::Rect_<double>(x - 1500, y - 1500, 3000, 3000));

            PatternProjection projection(intrisicMat, distortionCoefficients);
            projection.setPattern(&pattern, 0.8);
            cv::Mat image;
            image.create(1024, 1024, CV_64F);
            for (int i = 0; i <= 360; i++) {
                double theta = i * 2 * PI / 360.0;
                rVec.at<double>(2) = theta;

                tVec.at<double>(0) = -x * cos(theta) + y * sin(theta);
                tVec.at<double>(1) = -x * sin(theta) - y * cos(theta);

                projection.getImage(tVec, rVec, image);
                //cv::line(image, cv::Point(320, 0), cv::Point(320, 480), (255, 255, 255));
                //cv::line(image, cv::Point(0, 240), cv::Point(640, 240), (255, 255, 255));

                //cv::line(image, cv::Point(900-20, 700), cv::Point(900+20, 700), (255, 255, 255));
                //cv::line(image, cv::Point(900, 700-20), cv::Point(900, 700+20), (255, 255, 255));
                cv::imshow("Pattern projection", image);
                //cv::imwrite("test" + toString(i) + ".png", image);
                cv::waitKey(10);
            }
        }
    };
}

#endif /* TESTPATTERNPROJECTION_HPP */