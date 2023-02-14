/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTPOSE_HPP
#define TESTPOSE_HPP

#include "Vernier.hpp"

namespace Vernier {

    class TestPose {
    public:

        static void main() {

            Vernier::Pose pose2d(-3, -5, 0.2);

            std::cout << "Pattern pose definition : " << std::endl << pose2d.toString() << std::endl;

            std::cout << "Pattern pose in camera frame : " << std::endl << pose2d.getCameraToPatternTransformationMatrix() << std::endl;

            std::cout << "Camera pose in pattern frame : " << std::endl << pose2d.getPatternToCameraTransformationMatrix() << std::endl;


        }

        static void runAllTests() {
        }

        static double speed(unsigned long testCount) {
            return toc(0.0);
        }

    };
}

#endif