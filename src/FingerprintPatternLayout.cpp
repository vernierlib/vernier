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
//    void FingerprintPatternLayout::readJSON(rapidjson::Value & document) {
//
//        PatternLayout::readJSON(document);
//
//        if (document.HasMember("period") && document["period"].IsDouble()) {
//            period = document["period"].GetDouble();
//        } else {
//            throw Exception("The file is not a valid bitmap pattern file, the period is missing or has a wrong format.");
//        }
//        if (document.HasMember("bitmap") && document["bitmap"].IsArray()) {
//            nRows = document["bitmap"].Size();
//        } else {
//            throw Exception("The file is not a valid bitmap pattern file, the bitmap is missing or has a wrong format.");
//        }
//
//        if (document["bitmap"][0].IsArray() && document["bitmap"][0].Size() > 0) {
//            nCols = document["bitmap"][0].Size();
//        } else {
//            throw Exception("The file is not a valid bitmap pattern file, the first row of the bitmap has a wrong format.");
//        }
//
//        resize(period, nRows, nCols);
//        for (rapidjson::SizeType row = 0; row < nRows; row++) {
//            const rapidjson::Value& value = document["bitmap"][row];
//            if (value.IsArray() && value.Size() == nCols) {
//                for (rapidjson::SizeType col = 0; col < nCols; col++) {
//                    if (value[col].IsInt()) {
//                        bitmap(row, col) = value[col].GetInt();
//                    } else {
//                        Exception("The file is not a valid bitmap pattern file, the row " + to_string(row) + " of the bitmap has a wrong format");
//                    }
//                }
//            } else {
//                throw Exception("The file is not a valid bitmap pattern file, the row " + to_string(row) + " of the bitmap has a wrong size");
//            }
//        }
//    }

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
        if (x < -0.5 * width || y < -0.5 * height || x > 0.5 * width || y > 0.5 * height) {
            return 0;
        } else {
            int col = std::round((x + originX) / period);
            int row = std::round((y + originY) / period);
            if (col < 0 || row < 0 || col >= nCols || row >= nRows) {
                return 0;
            } else {
                double periodicIntensity = (1 + cos(2 * PI * x / period)) * (1 + cos(2 * PI * y / period)) / 4;
                return (bitmap(row, col) * periodicIntensity);
            }
        }
    }

}