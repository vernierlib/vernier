/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "StampMarkerLayout.hpp"

namespace vernier {

    StampMarkerLayout::StampMarkerLayout()
    : StampMarkerLayout(0.0, 1, 1) {
    }

    StampMarkerLayout::StampMarkerLayout(double period, int nRows, int nCols) : PeriodicPatternLayout() {
        classname = "FingerprintPattern";
        resize(period, nRows, nCols);
    }

    void StampMarkerLayout::resize(double period, int nRows, int nCols) {
        PeriodicPatternLayout::resize(period, nRows, nCols);
        bitmap.resize(nRows, nCols);
    }

    void StampMarkerLayout::writeJSON(std::ofstream & file) {
        PatternLayout::writeJSON(file);
        file << "        \"period\": " << period << "," << std::endl;
        file << "        \"bitmap\": [" << std::endl;
        for (int row = 0; row < bitmap.rows(); row++) {
            file << "            [";
            for (int col = 0; col < bitmap.cols(); col++) {
                if (col < bitmap.cols() - 1) {
                    file << bitmap(row, col) << ", ";
                } else {
                    file << bitmap(row, col);
                }
            }
            if (row < bitmap.rows() - 1) {
                file << "]," << std::endl;
            } else {
                file << "]" << std::endl;
            }
        }
        file << "        ]," << std::endl;
    }

    void StampMarkerLayout::readJSON(rapidjson::Value & document) {

        PatternLayout::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            period = document["period"].GetDouble();
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the period is missing or has a wrong format.");
        }
        if (document.HasMember("bitmap") && document["bitmap"].IsArray()) {
            nRows = document["bitmap"].Size();
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the bitmap is missing or has a wrong format.");
        }

        if (document["bitmap"][0].IsArray() && document["bitmap"][0].Size() > 0) {
            nCols = document["bitmap"][0].Size();
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the first row of the bitmap has a wrong format.");
        }

        resize(period, nRows, nCols);
        for (rapidjson::SizeType row = 0; row < nRows; row++) {
            const rapidjson::Value& value = document["bitmap"][row];
            if (value.IsArray() && value.Size() == nCols) {
                for (rapidjson::SizeType col = 0; col < nCols; col++) {
                    if (value[col].IsInt()) {
                        bitmap(row, col) = value[col].GetInt();
                    } else {
                        Exception("The file is not a valid bitmap pattern file, the row " + toString(row) + " of the bitmap has a wrong format");
                    }
                }
            } else {
                throw Exception("The file is not a valid bitmap pattern file, the row " + toString(row) + " of the bitmap has a wrong size");
            }
        }
    }



    void StampMarkerLayout::loadFromPNG(std::string filename, double period) {
#ifdef USE_OPENCV        
        cv::Mat image1 = cv::imread(filename, cv::IMREAD_GRAYSCALE), image;
        image1.convertTo(image, CV_8U);
        resize(period, image.rows, image.cols);
        description = "Bitmap pattern created from " + filename;

        bitmap = Eigen::ArrayXXi::Zero(image.rows, image.cols);
        for (int col = 0; col < image.cols; col++) {
            for (int row = 0; row < image.rows; row++) {
                bitmap(row, col) = (int) (image.at<char>(row, col) != 0);
            }
        }
#else
        std::cout << "OpenCV is required to load PNG files." << std::endl;
#endif // USE_OPENCV
    }

    void StampMarkerLayout::toRectangleVector(std::vector<Rectangle>& rectangleList) {
        rectangleList.clear();
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

    double StampMarkerLayout::getIntensity(double x, double y) {
        if (x < -0.5 * width || y < -0.5 * height || x > 0.5 * width || y > 0.5 * height) {
            return 0;
        } else {
            int col = std::floor((x + originX) / period);
            int row = std::floor((y + originY) / period);
            if (col < 0 || row < 0 || col >= nCols || row >= nRows) {
                return 0;
            } else {
                double periodicIntensity = (1 + cos(2 * PI * x / period)) * (1 + cos(2 * PI * y / period)) / 4;
                return (bitmap(row, col) * periodicIntensity);
            }
        }
    }

}