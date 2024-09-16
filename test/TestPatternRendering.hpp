/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTPATTERNRENDERING_HPP
#define TESTPATTERNRENDERING_HPP

//#include <opencv2/highgui.hpp>
#include "UnitTest.hpp"
#include "PatternRendering.hpp"
#include "FourierTransform.hpp"
#include "Spectrum.hpp"

#include <random>

namespace Vernier {

    class TestPatternRendering {
    public:

        static void main() {
            Eigen::MatioFile file("Megarena/newMask12Bits_x3_2pi.mat", MAT_ACC_RDONLY);
            Eigen::ArrayXXi bitSequence(1, 1);
            file.read_mat("bit_sequence_codee", bitSequence);
            MegarenaPatternForLayout pattern = MegarenaPatternForLayout(4.5, bitSequence, 0);

            PatternRendering renderer(&pattern);
            Eigen::ArrayXXd array(1024, 1024);

            renderer.getArray(0.5, 252, 252, 0, array);

            cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());
            cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
            //cv::line(image, cv::Point2d(512, 512), cv::Point2d(1024, 512), cv::Scalar(255, 255, 255));
            cv::imshow("Pattern projection", image);
            image.convertTo(image, CV_8UC3, 255.0);
            cv::imwrite("G:/Utilisateurs/antoine.andre/Desktop/test_" + toString(4) + ".png", image);

            //Graphics::drawHistogram(image);

            cv::waitKey(0);
        }

        static void main2() {
            //PeriodicPattern pattern = PeriodicPattern(5, 39, 39);

            QRCodePattern pattern = QRCodePattern(5, 37);

            PatternRendering renderer(&pattern);
            Eigen::ArrayXXd array(300, 300);

            renderer.getArray(1.2, 0, 0, 0.0, array);

            cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());
            cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
            //cv::line(image, cv::Point2d(512, 512), cv::Point2d(1024, 512), cv::Scalar(255, 255, 255));
            cv::imshow("Pattern projection", image);
            //cv::imwrite("test" + toString(1) + ".png", image);

            //Graphics::drawHistogram(image);

            cv::waitKey(0);
        }

        static void main3() {
            BitmapPatternLayout pattern = BitmapPatternLayout(5, 101, 101);
            PatternRendering renderer(&pattern);
            Eigen::ArrayXXd patternArray(1024, 1024);

            Eigen::Matrix3d intrinsinc;
            intrinsinc << 2500, 0, 0,
                    0, 2500, 0,
                    0, 0, 1;

            renderer.get3DArray(intrinsinc, 1, 256, 256, 0.7, 0.5, 0.1, 2500, patternArray);

            cv::Mat image(patternArray.rows(), patternArray.cols(), CV_64FC1, patternArray.data());
            cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
            cv::imshow("Pattern projection", image);
            cv::waitKey(0);
        }

        static void main4() {
            BitmapPatternLayout pattern = BitmapPatternLayout(5, 101, 101);
            PatternRendering renderer(&pattern);
            Eigen::ArrayXXd patternArray(1024, 1024);
            Eigen::ArrayXXcd patternArray2(1024, 1024);
            FourierTransform fft;
            Eigen::ArrayXXcd spectrum;
            Eigen::ArrayXXcd spectrumShifted;
            fft.resize(1024, 1024, FFTW_FORWARD);
            spectrumShifted.resize(1024, 1024);
            Eigen::Matrix3d intrinsinc;
            intrinsinc << 2500, 0, 0,
                    0, 2500, 0,
                    0, 0, 1;

            Eigen::Vector3d mainPeak1, mainPeak2;
            mainPeak1.setConstant(0);
            mainPeak2.setConstant(0);

            cv::Mat spectrumImage(1024, 1024, CV_64FC3);

            for (int i = 0; i < 1000; i++) {
                renderer.get3DArray(intrinsinc, 1, 0, 0, cos(i * PI / 180), cos(i * PI / 180) / 2, sin(i * PI / 180) / 2, 2500, patternArray);
                patternArray2 = patternArray;
                fft.compute(patternArray2, spectrum);
                Spectrum::shift(spectrum, spectrumShifted);
                spectrumImage = Spectrum::draw(spectrumShifted, mainPeak1, mainPeak2);
                cv::imshow("spectrum", spectrumImage);

                cv::Mat image(patternArray.rows(), patternArray.cols(), CV_64FC1, patternArray.data());
                cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
                cv::imshow("Pattern projection", image);
                cv::waitKey(1);
            }

            cv::waitKey(0);
        }

        static void mainReadFromPNG() {
            BitmapPatternLayout pattern = BitmapPatternLayout(10, 1, 1);
            pattern.loadFromPNG("femto2.png");

            PatternRendering renderer(&pattern);
            Eigen::ArrayXXd array(1024, 1024);

            renderer.getArray(2, 0, 0, 0.0, array);

            cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());
            cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
            cv::imshow("Pattern projection", image);

            cv::waitKey(0);
        }
    };
}

#endif /* TESTPATTERNPROJECTION_HPP */