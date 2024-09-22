/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "BitmapPatternLayout.hpp"

namespace Vernier {

    BitmapPatternLayout::BitmapPatternLayout()
    : BitmapPatternLayout(0.0, 1, 1) {
    }

    BitmapPatternLayout::BitmapPatternLayout(double period, int nRows, int nCols) : PeriodicPatternLayout() {
        classname = "BitmapPattern";
        resize(period, nRows, nCols);
    }
    
    BitmapPatternLayout::BitmapPatternLayout(std::string filename, double period) {
#ifdef USE_OPENCV        
        cv::Mat image1 = cv::imread(filename, cv::IMREAD_GRAYSCALE), image;
        image1.convertTo(image, CV_8U);
        resize(period, (image.rows + 1) / 2, (image.cols + 1) / 2);
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

    void BitmapPatternLayout::resize(double period, int nRows, int nCols) {
        PeriodicPatternLayout::resize(period, nRows, nCols);
        bitmap.resize(2 * nRows - 1, 2 * nCols - 1);
    }

    void BitmapPatternLayout::writeJSON(std::ofstream & file) {
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

    void BitmapPatternLayout::readJSON(rapidjson::Value & document) {

        PatternLayout::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            period = document["period"].GetDouble();
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the period is missing or has a wrong format.");
        }
        if (document.HasMember("bitmap") && document["bitmap"].IsArray()) {
            nRows = (document["bitmap"].Size()+1)/2;
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the bitmap is missing or has a wrong format.");
        }

        if (document["bitmap"][0].IsArray() && document["bitmap"][0].Size() > 0) {
            nCols = (document["bitmap"][0].Size()+1)/2;
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the first row of the bitmap has a wrong format.");
        }

        resize(period, nRows, nCols);
        for (rapidjson::SizeType row = 0; row < bitmap.rows(); row++) {
            const rapidjson::Value& value = document["bitmap"][row];
            if (value.IsArray() && value.Size() == bitmap.cols()) {
                for (rapidjson::SizeType col = 0; col < bitmap.cols(); col++) {
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

    void BitmapPatternLayout::toRectangleVector(std::vector<Rectangle>& rectangleList) {
        rectangleList.clear();
        for (int col = 0; col < bitmap.cols(); col++) {
            double x = col * dotSize;
            for (int row = 0; row < bitmap.rows(); row++) {
                double y = row * dotSize;
                if (bitmap(row, col)) {
                    rectangleList.push_back(Rectangle(x, y, dotSize, dotSize));
                }
            }
        }
    }

    double BitmapPatternLayout::getIntensity(double x, double y) {
        if (x < -0.5 * width || y < -0.5 * height || x > 0.5 * width || y > 0.5 * height) {
            return 0;
        } else {
            int col = std::floor((x + originX) / dotSize);
            int row = std::floor((y + originY) / dotSize);
            if (col < 0 || row < 0 || col >= bitmap.cols() || row >= bitmap.rows()) {
                return 0;
            } else {
                return bitmap(row, col); // *PeriodicPatternLayout::getIntensity(x,y);
            }
        }
    }

    int BitmapPatternLayout::numberOfWrongEdges() {
        int n = 0;
        for (int col = 0; col < bitmap.cols() - 1; col++) {
            for (int row = 0; row < bitmap.rows() - 1; row++) {
                n += (col % 2)*(bitmap(row, col) > bitmap(row, col + 1)) + ((col + 1) % 2)*(bitmap(row, col) < bitmap(row, col + 1));
                n += (row % 2)*(bitmap(row, col) > bitmap(row + 1, col)) + ((row + 1) % 2)*(bitmap(row, col) < bitmap(row + 1, col));
            }
        }
        return n;
    }

    bool BitmapPatternLayout::hasWrongEdges() {
        return (numberOfWrongEdges() != 0);
    }
    
    int BitmapPatternLayout::numberOfCorrectEdges() {
        int n = 0;
        for (int col = 0; col < bitmap.cols() - 1; col++) {
            for (int row = 0; row < bitmap.rows() - 1; row++) {
                n += (col % 2)*(bitmap(row, col) < bitmap(row, col + 1)) + ((col + 1) % 2)*(bitmap(row, col) > bitmap(row, col + 1));
                n += (row % 2)*(bitmap(row, col) < bitmap(row + 1, col)) + ((row + 1) % 2)*(bitmap(row, col) > bitmap(row + 1, col));
            }
        }
        return n;
    }

}