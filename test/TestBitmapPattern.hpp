/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef TESTBITMAPPATTERN_HPP
#define TESTBITMAPPATTERN_HPP

#include "Layout.hpp"
#include "eigen-matio/MatioFile.hpp"

namespace Vernier {

    class TestBitmapPattern {
    public:

        static void main() {
            /* This example converts a PNG image file into a bitmap pattern layout (JSON).
             */
            // Constructing the layout from PNG image file  
            double period = 21.0;
            BitmapPatternLayout layout;
            layout.loadFromPNG("femto.png", period);
            layout.setDescription("Example of bitmap pattern");

            // Writing the layout in a JSON file
            layout.saveToJSON("bitmapPattern.json");

            std::cout << "bitmapPattern.json generation completed." << std::endl;
        }

        static void runAllTests() {



        }


    };
}

#endif