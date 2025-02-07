/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Common.hpp"

namespace vernier {

    class Rectangle {
    public:

        double x, y, width, height;

        Rectangle();

        Rectangle(double x, double y, double width, double height);

        std::string toString();
    };
}

#endif