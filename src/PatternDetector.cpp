/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "PatternDetector.hpp"

namespace Vernier {

    PatternDetector::PatternDetector() {
    }

    void PatternDetector::readJSON(rapidjson::Value& document) {
        if (document.HasMember("description") && document["description"].IsString()) {
            description = document["description"].GetString();
        } else {
            description = "";
        }
        if (document.HasMember("date") && document["date"].IsString()) {
            date = document["date"].GetString();
        } else {
            date = "";
        }
        if (document.HasMember("author") && document["author"].IsString()) {
            author = document["author"].GetString();
        } else {
            author = "";
        }
        if (document.HasMember("unit") && document["unit"].IsString()) {
            unit = document["unit"].GetString();
        } else {
            unit = "";
        }
    }

    void PatternDetector::loadFromJSON(std::string filename) {
        BufferedReader bufferedReader(filename);

        rapidjson::Document document;
        document.ParseInsitu(bufferedReader.data());
        if (!document.IsObject()) {
            throw Exception(filename + " is not a valid JSON file.");
        }
        if (document.MemberBegin() == document.MemberEnd()) {
            throw Exception(filename + " is empty.");
        }
        readJSON(document.MemberBegin()->value);
    }

    void PatternDetector::compute(char* data, int rows, int cols) {
        Eigen::MatrixXd mPatternMatrix(rows, cols);
        std::memcpy(mPatternMatrix.data(), data, rows * cols * sizeof (double));

        Eigen::ArrayXXd patternImage = mPatternMatrix.array();
        compute(patternImage);
    }

#ifdef USE_OPENCV 
    void PatternDetector::compute(cv::Mat& image) {
        cv::Mat grayImage;
        if (image.channels() > 1) {
            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = image;
        }

        grayImage.convertTo(grayImage, CV_64F);
        cv::normalize(grayImage, grayImage, 1.0, 0, cv::NORM_MINMAX);
        
        Eigen::MatrixXd patternMatrix;
        cv::cv2eigen(grayImage, patternMatrix);
        Eigen::ArrayXXd patternArray(patternMatrix.rows(), patternMatrix.cols());
        compute(patternArray);
    }
#endif // USE_OPENCV

    void* PatternDetector::getObject(const std::string & attribute) {
            throw Exception("The attribute " + attribute + " is not accessible or defined in class " + classname + ".");
    }

    double PatternDetector::getDouble(const std::string & attribute) {
        throw Exception("The attribute " + attribute + " is not accessible or defined in class " + classname + ".");
    }

    double PatternDetector::getInt(const std::string & attribute) {
        throw Exception("The attribute " + attribute + " is not accessible or defined in class " + classname + ".");
    }

    std::string PatternDetector::getString(const std::string & attribute) {
        if (attribute == "classname") {
            return classname;
        } else if (attribute == "description") {
            return description;
        } else if (attribute == "date") {
            return date;
        } else if (attribute == "description") {
            return description;
        } else if (attribute == "author") {
            return author;
        } else if (attribute == "unit") {
            return unit;
        } else {
            throw Exception("The attribute " + attribute + " is not accessible or defined in class " + classname + ".");
        }
    }

    void PatternDetector::setDouble(const std::string & attribute, double value) {
        throw Exception("The attribute " + attribute + " is not accessible or defined in class " + classname + ".");
    }

    void PatternDetector::setInt(const std::string & attribute, int value) {
        throw Exception("The attribute " + attribute + " is not accessible or defined in class " + classname + ".");
    }

    void PatternDetector::setString(const std::string & attribute, std::string value) {
        if (attribute == "classname") {
            classname = value;
        } else if (attribute == "description") {
            description = value;
        } else if (attribute == "date") {
            date = value;
        } else if (attribute == "description") {
            description = value;
        } else if (attribute == "author") {
            author = value;
        } else if (attribute == "unit") {
            unit = value;
        } else {
            throw Exception("The attribute " + attribute + " is not accessible or defined in class " + classname + ".");
        }
    }

}

