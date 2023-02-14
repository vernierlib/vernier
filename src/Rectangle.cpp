/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "Rectangle.hpp"

namespace Vernier {

    Rectangle::Rectangle() : Rectangle(0.0, 0.0, 0.0, 0.0) {
    }

    Rectangle::Rectangle(double x, double y, double width, double height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    std::string Rectangle::toString() {
        return "[" + Vernier::toString(x) + "; " + Vernier::toString(y) + "; " + Vernier::toString(width) + "; " + Vernier::toString(height) + "]";

    }
}