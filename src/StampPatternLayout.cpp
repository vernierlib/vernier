/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

#include "StampPatternLayout.hpp"

namespace vernier {

    StampPatternLayout::StampPatternLayout()
    : StampPatternLayout(0.0, 1, 1) {
    }

    StampPatternLayout::StampPatternLayout(double period, int nRowsCols, int id) : BitmapPatternLayout(period, nRowsCols + 4, nRowsCols + 4) {
        if (id <= 0 || id >= 256) {
            throw Exception("The id must be between 1 and 255.");
        }
        this->id = id;
        classname = "StampPattern";
        description = "Stamp_s" + to_string(nRowsCols) + "_id" + to_string(id) + "_p" + to_string(period) + unit;
        bitmap = 0;
        for (int col = 0; col < bitmap.cols(); col++) {
            for (int row = 0; row < bitmap.rows(); row++) {
                if (row < 3 || col < 3 || col >= bitmap.cols() - 3 || row >= bitmap.rows() - 3) {
                    bitmap(row, col) = 1;
                } else if (row % 2 == 0 && col % 2 == 0) {
                    bitmap(row, col) = 1;
                } else
                    bitmap(row, col) = 0;
            }
        }

        int b = id;
        int row = 5;
        int col = 4;
        while (b != 0) {
            bitmap(row, col) = b % 2;
            bitmap(row + 1, bitmap.cols() - col - 2) = b % 2;
            b = b / 2;
            row += 2;
        }
        b = id;
        row = bitmap.rows() - 6;
        col = 6;
        while (b != 0) {
            bitmap(row, col) = b % 2;
            bitmap(row + 1, bitmap.cols() - col) = b % 2;
            b = b / 2;
            col += 2;
        }

    }

    void StampPatternLayout::writeJSON(std::ofstream & file) {
        throw Exception("Not implemented yet");
    }

    void StampPatternLayout::readJSON(const rapidjson::Value & document) {
        throw Exception("Not implemented yet");
    }


}