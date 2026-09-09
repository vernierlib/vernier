/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

#include "CheckerboardLayout.hpp"
#include <algorithm>

namespace vernier {

    CheckerboardLayout::CheckerboardLayout() : PatternLayout() {
        this->nRows = 0;
        this->nCols = 0;
        this->squareSize = 0.0;
    }

    CheckerboardLayout::CheckerboardLayout(double squareSize, int nRows, int nCols) : PatternLayout() {
        classname = "Checkerboard";
        resize(squareSize, nRows, nCols);
    }

    void CheckerboardLayout::resize(double squareSize, int nRows, int nCols) {
        if (nRows <= 0) {
            throw Exception("The number of rows must be positive.");
        }
        if (nCols <= 0) {
            throw Exception("The number of columns must be positive.");
        }
        if (squareSize <= 0.0) {
            throw Exception("The square size must be positive.");
        }
        this->nRows = nRows;
        this->nCols = nCols;
        this->squareSize = squareSize;
        width = nCols * squareSize;
        height = nRows * squareSize;
        originX = 0.0;
        originY = 0.0;
        description = "Checkerboard_s" + to_string(nRows) + "x" + to_string(nCols) + "_d" + to_string(squareSize) + unit;
    }

    void CheckerboardLayout::writeJSON(std::ofstream & file) {
        PatternLayout::writeJSON(file);
        file << "        \"squareSize\": " << squareSize << "," << std::endl;
        file << "        \"nRows\": " << nRows << "," << std::endl;
        file << "        \"nCols\": " << nCols << "," << std::endl;
    }

    void CheckerboardLayout::readJSON(const rapidjson::Value & document) {
        PatternLayout::readJSON(document);
        if (document.HasMember("squareSize") && document["squareSize"].IsDouble()) {
            squareSize = document["squareSize"].GetDouble();
        } else {
            throw Exception("The file is not a valid checkerboard pattern file, the square size is missing or has a wrong format.");
        }
        if (document.HasMember("nRows") && document["nRows"].IsInt()) {
            nRows = document["nRows"].GetInt();
        } else {
            throw Exception("The file is not a valid checkerboard pattern file, the number of rows (nRows) is missing or has a wrong format.");
        }
        if (document.HasMember("nCols") && document["nCols"].IsInt()) {
            nCols = document["nCols"].GetInt();
        } else {
            throw Exception("The file is not a valid checkerboard pattern file, the number of columns (nCols) is missing or has a wrong format.");
        }
        resize(squareSize, nRows, nCols);
    }

    void CheckerboardLayout::toRectangleVector(std::vector<Rectangle>& rectangleList) {
        PRINT(squareSize)
        for (int col = 0; col < nCols; col++) {
            double x = col * squareSize;
            for (int row = 0; row < nRows; row++) {
                double y = row * squareSize;
                if ((row + col) % 2 == 0) {
                    rectangleList.push_back(Rectangle(x, y, squareSize, squareSize));
                }
            }
        }
    }

    double CheckerboardLayout::getIntensity(double x, double y) {
        if (x < 0.0 || y < 0.0 || x > width || y > height) {
            return 0.0;
        } else if (((int) (x / squareSize) + (int) (y / squareSize)) % 2 == 0) {
            return 1.0;
        } else {
            return 0.0;
        }
    }

    double CheckerboardLayout::getPhase1(double x, double y) {
        throw Exception("getPhase1 is not implemented for " + this->classname);
        return 0.0;
    }

    double CheckerboardLayout::getPhase2(double x, double y) {
        throw Exception("getPhase2 is not implemented for " + this->classname);
        return 0.0;
    }

}