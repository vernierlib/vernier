/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTBITMAPPATTERN_HPP
#define TESTBITMAPPATTERN_HPP

#include "Layout.hpp"
#include "UnitTest.hpp"
#include "eigen-matio/MatioFile.hpp"

namespace vernier {

    class TestBitmapPattern {
    public:

        static void main() {
            /* This example converts a PNG image file into a bitmap pattern layout (JSON).
             */
            // Constructing the layout from PNG image file  
            double period = 21.0;
            BitmapPatternLayout layout("test.png", period);
            
            std::cout << "Number of wrong edges = " << layout.numberOfWrongEdges()<<  std::endl;       
            std::cout << "Number of correct edges = " << layout.numberOfCorrectEdges()<<  std::endl;
         
            // Writing the layout in a JSON file
            layout.saveToJSON("bitmapPattern.json");
            std::cout << "bitmapPattern.json generation completed." << std::endl;

            // Writing the layout in a SVG file
            layout.saveToSVG("bitmapPattern.svg");
            std::cout << "bitmapPattern.svg generation completed." << std::endl;

            // Setting the pose for rendering
            double x = 0.0;
            double y = 0.0;
            double alpha = 0.2;
            double beta = 0.0;
            double gamma = 0.0;
            double pixelSize = 1.0;
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