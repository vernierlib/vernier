/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#include "Rectangle.hpp"

namespace vernier {

    Rectangle::Rectangle() : Rectangle(0.0, 0.0, 0.0, 0.0) {
    }

    Rectangle::Rectangle(double x, double y, double width, double height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    std::string Rectangle::toString() {
        return "[" + vernier::toString(x) + "; " + vernier::toString(y) + "; " + vernier::toString(width) + "; " + vernier::toString(height) + "]";

    }
}