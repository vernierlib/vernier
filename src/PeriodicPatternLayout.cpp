/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "PeriodicPatternLayout.hpp"

namespace Vernier {

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
        width = period * nCols;
        height = period * nRows;
        originX = 0.5 * width;
        originY = 0.5 * height;
    }

    void PeriodicPatternLayout::writeJSON(std::ofstream & file) {
        PatternLayout::writeJSON(file);
        file << "        \"period\": " << period << "," << std::endl;
        file << "        \"nRows\": " << nRows << "," << std::endl;
        file << "        \"nCols\": " << nCols << "," << std::endl;
    }

    void PeriodicPatternLayout::readJSON(rapidjson::Value & document) {

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
    }

    void PeriodicPatternLayout::toRectangleVector(std::vector<Rectangle>& rectangleList) {
        rectangleList.clear();
        for (int col = 0; col < nCols; col++) {
            double x = col * period;
            for (int row = 0; row < nRows; row++) {
                double y = row * period;
                rectangleList.push_back(Rectangle(x, y, dotSize, dotSize));
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
#ifdef USE_OPENCV        
        cv::Mat image(2 * nRows, 2 * nCols, CV_8U);
        for (int col = 0; col < image.cols; col++) {
            double x = col * dotSize - originX;
            for (int row = 0; row < image.rows; row++) {
                double y = row * dotSize - originY;
                image.at<char>(row, col) = (char) (255 * (getIntensity(x, y) > 0.5));
            }
        }
        if (filename == "") {
            filename = classname + ".png";
        }
        cv::imwrite(filename, image);
#else
        std::cout << "OpenCV is required to save PNG files." << std::endl;
#endif // USE_OPENCV
    }

}