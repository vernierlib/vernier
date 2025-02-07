/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UFC.
 */

#include "MegarenaPatternLayout.hpp"
#include "MegarenaBitSequence.hpp"

namespace vernier {

    MegarenaPatternLayout::MegarenaPatternLayout()
    : PeriodicPatternLayout() {
        classname = "MegarenaPattern";
    }

    MegarenaPatternLayout::MegarenaPatternLayout(double period, Eigen::ArrayXXi & bitSequence)
    : PeriodicPatternLayout() {
        classname = "MegarenaPattern";
        description = "Custom code MegarenaPattern";
        resize(period, bitSequence);
    }

    MegarenaPatternLayout::MegarenaPatternLayout(double period, int codeSize)
    : PeriodicPatternLayout() {
        MegarenaBitSequence::generate(codeSize, bitSequence);
        if (period < 0.0) {
            throw Exception("The period must be positive.");
        }
        classname = "MegarenaPattern";
        description = toString(codeSize) + "-bits MegarenaPattern";
        resize(period, bitSequence);
    }

    void MegarenaPatternLayout::resize(double period, Eigen::ArrayXXi & bitSequence) {
        if (bitSequence.rows() != 1) {
            throw Exception("The bit sequence must have a single row");
        }
        if (bitSequence.cols() <= 0) {
            throw Exception("The bit sequence must have at least one column.");
        }

        this->period = period;
        this->dotSize = 0.5 * period;
        this->nRows = bitSequence.cols();
        this->nCols = bitSequence.cols();
        width = period * nCols - dotSize;
        height = period * nRows - dotSize;
        this->bitSequence = bitSequence;
        originX = 0.5 * dotSize;
        originY = 0.5 * dotSize;
        regionOfInterest = Rectangle(0.0, 0.0, width, height);
    }

    void MegarenaPatternLayout::writeJSON(std::ofstream & file) {
        PatternLayout::writeJSON(file);
        file << "        \"period\": " << period << "," << std::endl;
        file << "        \"regionOfInterest\": [" << regionOfInterest.x << ", " << regionOfInterest.y << ", " << regionOfInterest.width << ", " << regionOfInterest.height << " ]," << std::endl;
        file << "        \"bitSequence\": [";
        for (int col = 0; col < bitSequence.cols(); col++) {
            if (col < bitSequence.cols() - 1) {
                file << bitSequence(col) << ", ";
            } else {
                file << bitSequence(col);
            }
        }
        file << "]," << std::endl;
    }

    void MegarenaPatternLayout::readJSON(rapidjson::Value & document) {
        PatternLayout::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            period = document["period"].GetDouble();
        } else {
            throw Exception("The file is not a valid megarena pattern file, the period is missing or has a wrong format.");
        }

        if (document.HasMember("bitSequence") && document["bitSequence"].IsArray()) {
            bitSequence.resize(1, document["bitSequence"].Size());
            for (rapidjson::SizeType col = 0; col < document["bitSequence"].Size(); col++) {
                if (document["bitSequence"][col].IsInt()) {
                    bitSequence(col) = document["bitSequence"][col].GetInt();
                } else {
                    Exception("The file is not a valid megarena pattern file, the col " + toString(col) + " of the bitSequence has a wrong format");
                }
            }
        } else if (document.HasMember("codeSize") && document["codeSize"].IsInt()) {
            int codeSize = document["codeSize"].GetInt();
            if (codeSize >= 4 && codeSize <=12) {
                MegarenaBitSequence::generate(codeSize, bitSequence);
            } else {
                throw Exception("The file is not a valid megarena pattern file, the code size must between 4 and 12.");
            }
        } else {
            throw Exception("The file is not a valid megarena pattern file, the code size is missing or has a wrong format.");
        }
        resize(period, bitSequence);

        if (document.HasMember("regionOfInterest") && document["regionOfInterest"].IsArray()) {
            if (document["regionOfInterest"].Size() == 4 && document["regionOfInterest"][0].IsDouble() && document["regionOfInterest"][1].IsDouble() && document["regionOfInterest"][2].IsDouble() && document["regionOfInterest"][3].IsDouble()) {
                regionOfInterest.x = document["regionOfInterest"][0].GetDouble();
                regionOfInterest.y = document["regionOfInterest"][1].GetDouble();
                regionOfInterest.width = document["regionOfInterest"][2].GetDouble();
                regionOfInterest.height = document["regionOfInterest"][3].GetDouble();
            } else {
                throw Exception("The file is not a valid megarena pattern file, the regionOfInterest has a wrong format.");
            }
        }
    }

    void MegarenaPatternLayout::toRectangleVector(std::vector<Rectangle>& rectangleList) {
        int colStart = (int) (regionOfInterest.x / period);
        int colStop = (int) ((regionOfInterest.x + regionOfInterest.width) / period);
        int rowStart = (int) (regionOfInterest.y / period);
        int rowStop = (int) ((regionOfInterest.y + regionOfInterest.height) / period);

        if (colStart < 0) {
            colStart = 0;
        }
        if (rowStart < 0) {
            rowStart = 0;
        }
        if (colStop > nCols) {
            colStop = nCols;
        }
        if (rowStop > nRows) {
            rowStop = nRows;
        }
        for (int col = colStart; col < colStop; col++) {
            for (int row = rowStart; row < rowStop; row++) {
                if (bitSequence(row) && bitSequence(col) && (col % 3 != 0 || row % 3 != 0)) {
                    rectangleList.push_back(Rectangle(col * period, row * period, dotSize, dotSize));
                }
            }
        }
    }

    double MegarenaPatternLayout::getIntensity(double x, double y) {
        if (x<-0.5 * period || y<-0.5 * period || x > width || y > height) {
            return 0;
        } else {
            int col = (int)((x + dotSize) / period) ;
            int row = (int)((y + dotSize) / period) ;
            if (col < 0 || row < 0 || col >= nCols || row >= nRows) {
                return 0;
            } else {
                if (bitSequence(row) && bitSequence(col) && (col % 3 != 0 || row % 3 != 0)) {
                    return (1 + cos(2 * PI * x / period)) * (1 + cos(2 * PI * y / period)) / 4;
                } else {
                    return 0;
                }
            }
        }
    }

    void MegarenaPatternLayout::saveToPNG(std::string filename) {
#ifdef USE_OPENCV
        int colStart = (int) (regionOfInterest.x / dotSize);
        int colStop = (int) ((regionOfInterest.x + regionOfInterest.width) / dotSize);
        int rowStart = (int) (regionOfInterest.y / dotSize);
        int rowStop = (int) ((regionOfInterest.y + regionOfInterest.height) / dotSize);

        cv::Mat image(rowStop - rowStart, colStop - colStart, CV_8U);
        for (int col = colStart; col < colStop; col++) {
            double x = col * dotSize - originX;
            for (int row = rowStart; row < rowStop; row++) {
                double y = row * dotSize - originY;
                image.at<char>(row - rowStart, col - colStart) = (char) (255 * (getIntensity(x, y) > 0.5));
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

    Rectangle MegarenaPatternLayout::getRegionOfInterest() {
        return regionOfInterest;
    }

    void MegarenaPatternLayout::setRegionOfInterest(Rectangle regionOfInterest) {
        this->regionOfInterest = regionOfInterest;
    }

    void* MegarenaPatternLayout::getObject(const std::string & attribute) {
        if (attribute == "regionOfInterest") {
            return &regionOfInterest;
        } else if (attribute == "bitSequence") {
            return &bitSequence;
        } else {
            return PatternLayout::getObject(attribute);
        }
    }

}