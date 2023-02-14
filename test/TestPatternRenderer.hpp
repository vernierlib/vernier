/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTPATTERNRENDERER_HPP
#define TESTPATTERNRENDERER_HPP

#include "Layout.hpp"
#include "PatternRenderer.hpp"

#include <random>

namespace Vernier {

    class TestPatternRenderer {
    public:

#ifdef USE_OPENCV

        static void main() {
            PatternLayout *patternLayout;
            patternLayout = Layout::loadFromJSON("testMegarenaPattern3.json");

            PatternRenderer patternRenderer(patternLayout, 30 * 18. / 1024, 1);

            Eigen::ArrayXXd array(1024, 1024);

            patternRenderer.renderOrthographicProjection(Pose(0, 0, 1000, 0.0, 0, 0.0), array);

            cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());
            cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
            cv::imshow(patternLayout->getClassname(), image);
            cv::waitKey(0);
        }

        static void main2() {
            PatternLayout *patternLayout;
            patternLayout = Layout::loadFromJSON("testMegarenaPattern.json");

            PatternRenderer patternRenderer(patternLayout, 35. / 1024, 100);

            Eigen::ArrayXXd array(1024, 1024);

            patternRenderer.renderPerspectiveProjection(Pose(0, 0, 1000, 1.0, 0, 1.0), array);

            cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());
            cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
            cv::imshow(patternLayout->getClassname(), image);
            cv::waitKey(0);
        }

        static void main3() {
            PatternLayout *patternLayout;
            patternLayout = Layout::loadFromJSON("testPeriodicPattern.json");

            PatternRenderer patternRenderer(patternLayout, 35. / 1024, 1);

            Eigen::ArrayXXd array(10, 10);

            patternRenderer.renderPerspectiveProjection(Pose(0, 0, 100, 0.0, 0, 0.0), array);

            std::cout << array << std::endl;

            cv::Mat image(array.rows(), array.cols(), CV_64FC1, array.data());
            cv::normalize(image, image, 1, 0, cv::NORM_MINMAX);
            cv::imshow(patternLayout->getClassname(), image);
            cv::waitKey(60000);
        }
#endif // USE_OPENCV      


    };
}

#endif