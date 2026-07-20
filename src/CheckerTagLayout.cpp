/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

#include "CheckerTagLayout.hpp"

namespace vernier {

    CheckerTagLayout::CheckerTagLayout()
    : CheckerTagLayout(0.0, 1, 1) {
    }

    CheckerTagLayout::CheckerTagLayout(double dotSize, int nRowsCols, int id) : StampPatternLayout(dotSize, nRowsCols, id) {
        classname = "CheckerTag";
        this->period = sqrt(2.0) * dotSize;
        this->dotSize = dotSize;
        this->nRows = this->nCols = nRowsCols + 4;
        width = height = dotSize * nRows;
        originX = originY = 0.5 * width;
        description = "CheckerTag_s" + to_string(nRowsCols) + "_id" + to_string(id) + "_d" + to_string(dotSize) + unit;
        bitmap.resize(nRows, nCols);
        bitmap = 0;
        for (int col = 0; col < bitmap.cols(); col++) {
            for (int row = 0; row < bitmap.rows(); row++) {
                if (row < 2 || col < 2 || col >= bitmap.cols() - 2 || row >= bitmap.rows() - 2) {
                    bitmap(row, col) = 1;
                } else if ((row + col) % 2 == 0) {
                    bitmap(row, col) = 1;
                } else
                    bitmap(row, col) = 0;
            }
        }

        int b = id;
        int row = 3;
        int col = 2;
        while (b != 0) {
            bitmap(row, col) = b % 2;
            bitmap(row + 1, bitmap.cols() - col - 2) = b % 2;
            b = b / 2;
            row += 2;
        }
        b = id;
        row = bitmap.rows() - 4;
        col = 4;
        while (b != 0) {
            bitmap(row, col) = b % 2;
            bitmap(row + 1, bitmap.cols() - col) = b % 2;
            b = b / 2;
            col += 2;
        }
    }

    void CheckerTagLayout::writeJSON(std::ofstream & file) {
        throw Exception("Not implemented yet");
    }

    void CheckerTagLayout::readJSON(const rapidjson::Value & document) {
        throw Exception("Not implemented yet");
    }

}