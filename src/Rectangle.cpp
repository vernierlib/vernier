/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
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
        return "[" + vernier::to_string(x) + "; " + vernier::to_string(y) + "; " + vernier::to_string(width) + "; " + vernier::to_string(height) + "]";

    }
}