/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "CustomPatternLayout.hpp"
#include <algorithm>

namespace vernier {

    CustomPatternLayout::CustomPatternLayout() : PatternLayout() {
        classname = "CustomPattern";
        originX = 0.0;
        originY = 0.0;
    }

    void CustomPatternLayout::resize() {
        width = dots[0].x + dots[0].width;
        height = dots[0].y + dots[0].height;
        for (int i = 1; i < dots.size(); i++) {
            if (dots[i].x + dots[i].width > width) {
                width = dots[i].x + dots[i].width;
            }
            if (dots[i].y + dots[i].height > height) {
                height = dots[i].y + dots[i].height;
            }
        }
    }

    void CustomPatternLayout::writeJSON(std::ofstream & file) {
        PatternLayout::writeJSON(file);
        file << "        \"dots\": [" << std::endl;
        for (int i = 0; i < dots.size(); i++) {
            file << "            [" << dots[i].x << ", " << dots[i].y << ", " << dots[i].width << ", " << dots[i].height << ", " << dotsIntensity[i];
            if (i < dots.size() - 1) {
                file << "]," << std::endl;
            } else {
                file << "]" << std::endl;
            }
        }
        file << "        ]," << std::endl;
    }

    void CustomPatternLayout::readJSON(rapidjson::Value & document) {
        PatternLayout::readJSON(document);
        int nRows = 0;
        if (document.HasMember("dots") && document["dots"].IsArray()) {
            nRows = document["dots"].Size();
            dots.resize(nRows);
            dotsIntensity.resize(nRows);
        } else {
            throw Exception("The file is not a valid custom pattern file, the array of dots is missing or has a wrong format.");
        }

        if (document["dots"][0].IsArray() && document["dots"][0].Size() == 5) {
            ;
        } else {
            throw Exception("The file is not a valid custom pattern file, the first row of the array of dots  has a wrong format.");
        }

        for (rapidjson::SizeType row = 0; row < nRows; row++) {
            const rapidjson::Value& value = document["dots"][row];
            if (value.IsArray() && value.Size() == 5) {
                if (value[0].IsDouble() && value[1].IsDouble() && value[2].IsDouble() && value[3].IsDouble() && value[4].IsDouble()) {
                    dots[row] = Rectangle(value[0].GetDouble(), value[1].GetDouble(), value[2].GetDouble(), value[3].GetDouble());
                    dotsIntensity[row] = value[4].GetDouble();
                } else {
                    Exception("The file is not a valid custom pattern file, the row " + toString(row) + " of the array of dots has a wrong format");
                }
            } else {
                throw Exception("The file is not a valid custom pattern file, the row " + toString(row) + " of the array of dots has a wrong size");
            }
        }

        resize();
    }

    void CustomPatternLayout::loadFromCSV(std::string filename) {
        dots.clear();
        dotsIntensity.clear();

        std::ifstream file(filename);
        if (!file.good()) {
            throw Exception("File not found or not accessible.");
        }

        std::string line = "";
        std::getline(file, line);

        double x, y, w, h, intensity;
        while (!file.eof()) {
            std::getline(file, line);
            if (!file.fail() && !line.empty()) {
                std::replace(line.begin(), line.end(), ';', ' ');
                std::stringstream stream(line);
                stream >> x >> y >> w >> h >> intensity;
                dots.push_back(Rectangle(x, y, w, h));
                dotsIntensity.push_back(intensity);
            }
        }
        file.close();

        if (dots.size() == 0) {
            throw Exception(filename + " is not a valid CSV file.");
        }

        resize();
        description = "Custom pattern loaded from " + filename;
    }

    void CustomPatternLayout::toRectangleVector(std::vector<Rectangle>& rectangleList) {
        rectangleList = dots;
    }

    double CustomPatternLayout::getIntensity(double x, double y) {
        double intensity = 0.0;
        for (int i = 0; i < dots.size(); i++) {
            if (x >= dots[i].x - 0.5 * dots[i].width &&
                    x <= dots[i].x + 1.5 * dots[i].width &&
                    y >= dots[i].y - 0.5 * dots[i].height &&
                    y <= dots[i].y + 1.5 * dots[i].height) {
                intensity += (1 + cos(PI * (x - (dots[i].x + 0.5 * dots[i].width)) / dots[i].width))
                        * (1 + cos(PI * (y - (dots[i].y + 0.5 * dots[i].height)) / dots[i].height)) / 4;
            }
        }
        return intensity;
    }

}