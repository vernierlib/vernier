/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTFINGERPRINTPATTERN_HPP
#define TESTFINGERPRINTPATTERN_HPP

#include "Layout.hpp"
#include "UnitTest.hpp"

#include "eigen-matio/MatioFile.hpp"

namespace Vernier {

    class TestFingerprintPattern {
    public:

        static void main() {
            /* This example converts a PNG image file into a finger print pattern layout (JSON).
             */
            // Constructing the layout from PNG image file  
            double period = 21.0;
            FingerprintPatternLayout layout;
            layout.loadFromPNG("femto.png", period);
            layout.setDescription("Example of finger print pattern");

            // Writing the layout in a JSON file
            layout.saveToJSON("fingerprintPattern.json");

            std::cout << "bitmapPattern.json generation completed." << std::endl;

            // Setting the pose for rendering
            double x = 0.0;
            double y = 0.0;
            double alpha = 0.2;
            double beta = 0.3;
            double gamma = 0.4;
            double pixelSize = 4.0;
            Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);

            // Rendering
            Eigen::ArrayXXd array(512, 512);
            layout.renderOrthographicProjection(patternPose, array);

            // Showing the array
            arrayShow(layout.getDescription(), array);


        }

        static void runAllTests() {



        }


    };
}

#endif