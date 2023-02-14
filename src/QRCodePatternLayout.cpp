/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "QRCodePatternLayout.hpp"

namespace Vernier {

    QRCodePatternLayout::QRCodePatternLayout() : QRCodePatternLayout(0.0, 1) {
    }

    QRCodePatternLayout::QRCodePatternLayout(double period, int size)
    : PeriodicPatternLayout() {
        classname = "QRCodePattern";
        resize(period, size);
    }

    void QRCodePatternLayout::resize(double period, int size) {
        PeriodicPatternLayout::resize(period, size, size);
    }

    void QRCodePatternLayout::writeJSON(std::ofstream & file) {
        PatternLayout::writeJSON(file);
        file << "        \"period\": " << period << "," << std::endl;
        file << "        \"size\": " << nRows << "," << std::endl;
    }

    void QRCodePatternLayout::readJSON(rapidjson::Value & document) {

        PatternLayout::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            period = document["period"].GetDouble();
        } else {
            throw Exception("The file is not a valid QR code pattern file, the period is missing or has a wrong format.");
        }
        if (document.HasMember("size") && document["size"].IsInt()) {
            nRows = document["size"].GetInt();
        } else {
            throw Exception("The file is not a valid QR code pattern file, the size is missing or has a wrong format.");
        }
        resize(period, nRows);
    }

    void QRCodePatternLayout::addMarker(int row, int col, std::vector<Rectangle>& rectangleList) {
        rectangleList.push_back(Rectangle((2 * row + 0) * dotSize, (2 * col + 1) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 0) * dotSize, (2 * col + 3) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 0) * dotSize, (2 * col + 5) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 6) * dotSize, (2 * col + 1) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 6) * dotSize, (2 * col + 3) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 6) * dotSize, (2 * col + 5) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 1) * dotSize, (2 * col + 0) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 3) * dotSize, (2 * col + 0) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 5) * dotSize, (2 * col + 0) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 1) * dotSize, (2 * col + 6) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 3) * dotSize, (2 * col + 6) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 5) * dotSize, (2 * col + 6) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 3) * dotSize, (2 * col + 2) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 3) * dotSize, (2 * col + 3) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 3) * dotSize, (2 * col + 4) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 2) * dotSize, (2 * col + 3) * dotSize, dotSize, dotSize));
        rectangleList.push_back(Rectangle((2 * row + 4) * dotSize, (2 * col + 3) * dotSize, dotSize, dotSize));
    }

    void QRCodePatternLayout::toRectangleVector(std::vector<Rectangle>& rectangleList) {
        PeriodicPatternLayout::toRectangleVector(rectangleList);
        if (nRows > 4) {
            addMarker(0, 0, rectangleList);
        }
        if (nRows > 13) {
            addMarker(nRows - 4, 0, rectangleList);
            addMarker(0, nCols - 4, rectangleList);
        }
    }

    double QRCodePatternLayout::getIntensity(double x, double y) {
        if (x < -0.5 * width || y < -0.5 * height || x > 0.5 * width || y > 0.5 * height) {
            return 0;
        } else {
            int col = std::floor((x + originX - 0.25 * period) / dotSize);
            int row = std::floor((y + originY - 0.25 * period) / dotSize);
            if (((col >= 0 && row >= 0 && col < 7 && row < 7) && !(col >= 1 && row >= 1 && col < 6 && row < 6)) ||
                    (col >= 2 && row >= 2 && col < 5 && row < 5) ||

                    ((col >= 2 * nCols - 8 && row >= 0 && col < 2 * nCols - 1 && row < 7) && !(col >= 2 * nCols - 7 && row >= 1 && col < 2 * nCols - 2 && row < 6)) ||
                    (col >= 2 * nCols - 6 && row >= 2 && col < 2 * nCols - 3 && row < 5) ||

                    ((col >= 0 && row >= 2 * nRows - 8 && col < 7 && row < 2 * nRows - 1) && !(col >= 1 && row >= 2 * nRows - 7 && col < 6 && row < 2 * nRows - 2)) ||
                    (col >= 2 && row >= 2 * nRows - 6 && col < 5 && row < 2 * nRows - 3)

                    ) {
                return 1;
            } else {
                return PeriodicPatternLayout::getIntensity(x, y);
            }
        }
    }

}