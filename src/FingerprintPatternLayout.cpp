/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "FingerprintPatternLayout.hpp"

namespace vernier {

    FingerprintPatternLayout::FingerprintPatternLayout()
    : FingerprintPatternLayout(0.0, 1, 1) {
    }

    FingerprintPatternLayout::FingerprintPatternLayout(double period, int nRows, int nCols) : BitmapPatternLayout() {
        classname = "FingerprintPattern";
        resize(period, nRows, nCols);
    }

    FingerprintPatternLayout::FingerprintPatternLayout(std::string filename, double period) : BitmapPatternLayout(filename, period) {
        classname = "FingerprintPattern";
        PeriodicPatternLayout::resize(period, bitmap.rows(), bitmap.cols());
    }

    void FingerprintPatternLayout::resize(double period, int nRows, int nCols) {
        PeriodicPatternLayout::resize(period, nRows, nCols);
        bitmap.resize(nRows, nCols);
    }

    void FingerprintPatternLayout::readJSON(rapidjson::Value & document) {
        BitmapPatternLayout::readJSON(document);
        PeriodicPatternLayout::resize(period, bitmap.rows(), bitmap.cols());
    }

    void FingerprintPatternLayout::toRectangleVector(std::vector<Rectangle>& rectangleList) {
        for (int col = 0; col < bitmap.cols(); col++) {
            double x = col * period;
            for (int row = 0; row < bitmap.rows(); row++) {
                double y = row * period;
                if (bitmap(row, col)) {
                    rectangleList.push_back(Rectangle(x, y, dotSize, dotSize));
                }
            }
        }
    }

    double FingerprintPatternLayout::getIntensity(double x, double y) {
        int col = std::round((x + originX) / dotSize - 0.5);
        int row = std::round((y + originY) / dotSize - 0.5);

        if (col < 0 || row < 0 || col >= 2 * bitmap.cols() || row >= 2 * bitmap.rows()) {
            return 0;
        } else if (row % 2 == 1 || col % 2 == 1) {
            return 0;
        } else {
            double periodicIntensity = (1 + cos(2 * PI * x / dotSize)) * (1 + cos(2 * PI * y / dotSize)) / 4;
            return bitmap(row / 2, col / 2) * periodicIntensity;
        }
    }

    int FingerprintPatternLayout::numberOfCorrectEdges() {
        int n = 0;
        for (int col = 0; col < bitmap.cols() - 1; col++) {
            for (int row = 0; row < bitmap.rows() - 1; row++) {
                n += 4 * bitmap(row, col);
            }
        }
        return n;
    }

}