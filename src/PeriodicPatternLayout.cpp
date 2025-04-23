/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "PeriodicPatternLayout.hpp"

namespace vernier {

    PeriodicPatternLayout::PeriodicPatternLayout() : PeriodicPatternLayout(0.0, 1, 1) {
    }

    PeriodicPatternLayout::PeriodicPatternLayout(double period, int nRows, int nCols)
    : PatternLayout() {
        classname = "PeriodicPattern";
        resize(period, nRows, nCols);
    }

    void PeriodicPatternLayout::resize(double period, int nRows, int nCols) {
        if (nRows <= 0 || nRows % 2 == 0) {
            throw Exception("The number of rows must be positive and odd.");
        }
        if (nCols <= 0 || nCols % 2 == 0) {
            throw Exception("The number of columns must be positive and odd.");
        }
        this->period = period;
        this->dotSize = 0.5 * period;
        this->nRows = nRows;
        this->nCols = nCols;
        width = period * (nCols - 0.5);
        height = period * (nRows - 0.5);
        originX = 0.5 * width;
        originY = 0.5 * height;
    }

    void PeriodicPatternLayout::writeJSON(std::ofstream & file) {
        PatternLayout::writeJSON(file);
        file << "        \"period\": " << period << "," << std::endl;
        file << "        \"nRows\": " << nRows << "," << std::endl;
        file << "        \"nCols\": " << nCols << "," << std::endl;
    }

    void PeriodicPatternLayout::readJSON(const rapidjson::Value & document) {

        PatternLayout::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            period = document["period"].GetDouble();
        } else {
            throw Exception("The file is not a valid periodic pattern file, the period is missing or has a wrong format.");
        }
        if (document.HasMember("nRows") && document["nRows"].IsInt()) {
            nRows = document["nRows"].GetInt();
        } else {
            throw Exception("The file is not a valid periodic pattern file, the number of rows (nRows) is missing or has a wrong format.");
        }
        if (document.HasMember("nCols") && document["nCols"].IsInt()) {
            nCols = document["nCols"].GetInt();
        } else {
            throw Exception("The file is not a valid periodic pattern file, the number of columns (nCols) is missing or has a wrong format.");
        }
        resize(period, nRows, nCols);

        if (document.HasMember("dotSize") && document["dotSize"].IsDouble()) {
            dotSize = document["dotSize"].GetDouble();
        }
    }

    void PeriodicPatternLayout::toRectangleVector(std::vector<Rectangle>& rectangleList) {
        double offset = (period / 2 - dotSize) / 2;
        for (int col = 0; col < nCols; col++) {
            double x = col * period + offset;
            for (int row = 0; row < nRows; row++) {
                double y = row * period + offset;
                if (row != 0 || col != 0) {
                    rectangleList.push_back(Rectangle(x, y, dotSize, dotSize));
                }
            }
        }
    }

    double PeriodicPatternLayout::getIntensity(double x, double y) {
        if (x < -0.5 * width || y < -0.5 * height || x > 0.5 * width || y > 0.5 * height) {
            return 0;
        } else if (x < -0.5 * width + period && y < -0.5 * height + period) {
            return 0;
        } else {
            return (1 + cos(2 * PI * x / period)) * (1 + cos(2 * PI * y / period)) / 4;
        }
    }

    void PeriodicPatternLayout::saveToPNG(std::string filename) {
        cv::Mat image(2 * nRows - 1, 2 * nCols - 1, CV_8U);
        for (int col = 0; col < image.cols; col++) {
            double x = col * period * 0.5 + 0.25 * period - originX;
            for (int row = 0; row < image.rows; row++) {
                double y = row * period * 0.5 + 0.25 * period - originY;
                image.at<unsigned char>(row, col) = (unsigned char) (255 * (getIntensity(x, y) > 0.5));
            }
        }
        if (filename == "") {
            filename = classname + ".png";
        }
        cv::imwrite(filename, image);
    }

    std::string PeriodicPatternLayout::toString() {
        return PatternLayout::toString() + "\n  period: " + to_string(this->period) + unit + "\n  dotSize: " + to_string(dotSize) + unit + "\n  width: " + to_string(width) + unit + "\n  height: " + to_string(height) + unit;
    }

    double PeriodicPatternLayout::getPeriod() {
        return period;
    }

    int PeriodicPatternLayout::getNRows() {
        return nRows;
    }

    int PeriodicPatternLayout::getNCols() {
        return nCols;
    }

    double PeriodicPatternLayout::getDouble(const std::string & attribute) {
        if (attribute == "physicalPeriod" || attribute == "period") {
            return period;
        } else if (attribute == "dotSize") {
            return dotSize;
        } else {
            return PatternLayout::getDouble(attribute);
        }
    }

    int PeriodicPatternLayout::getInt(const std::string & attribute) {
        if (attribute == "nRows") {
            return nRows;
        } else if (attribute == "nCols") {
            return nCols;
        } else {
            return PatternLayout::getInt(attribute);
        }
    }

    void PeriodicPatternLayout::setDouble(const std::string & attribute, double value) {
        if (attribute == "dotSize") {
            dotSize = value;
        } else {
            PatternLayout::setDouble(attribute, value);
        }
    }

}