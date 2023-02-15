/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "PatternLayout.hpp"

namespace Vernier {

    PatternLayout::PatternLayout() {
        classname = "PatternLayout";
        description = "Pattern created with Vernier library";
        date = "2022";
        author = "FEMTO-ST";
        unit = "micrometer";
        originX = 0.0;
        originY = 0.0;
        margin = 0.0;
        width = 0.0;
        height = 0.0;
    }

    void PatternLayout::saveToSVG(std::string filename) {
        if (filename == "") {
            filename = classname + ".svg";
        }

        std::ofstream file;
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw Exception("Error creating file.");
        }

        file.precision(15);
        file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
        file << "<!-- Created with Vernier library -->" << std::endl;
        file << "<svg" << std::endl;
        file << "    xmlns=\"http://www.w3.org/2000/svg\"" << std::endl;
        file << "    version=\"1.1\"" << std::endl;
        file << "    x=\"0\" y=\"0\" width=\"" << width + 2 * margin << "\" height=\"" << height + 2 * margin << "\"" << std::endl;
        file << "    viewBox=\"0 0 " << width + 2 * margin << " " << height + 2 * margin << "\">" << std::endl;
        file << "<title>" << description << "</title>" << std::endl;
        file << "<desc>" << std::endl;
        file << "    class: " << classname << std::endl;
        file << "    description: " << description << std::endl;
        file << "    date: " << date << std::endl;
        file << "    author: " << author << std::endl;
        file << "    unit: " << unit << std::endl;
        file << "    width: " << width << std::endl;
        file << "    height: " << height << std::endl;
        //        file << "    period: " << period << std::endl;
        file << "    originX: " << originX << std::endl;
        file << "    originY: " << originY << std::endl;
        file << "    margin: " << margin << std::endl;
        file << "</desc>" << std::endl;
        std::vector<Rectangle> rectangleList;
        toRectangleVector(rectangleList);
        for (int i = 0; i < rectangleList.size(); i++) {
            file << "<rect x=\"" << rectangleList[i].x + margin << "\" ";
            file << "y=\"" << rectangleList[i].y + margin << "\" ";
            file << "width=\"" << rectangleList[i].width << "\" ";
            file << "height=\"" << rectangleList[i].height << "\" ";
            file << "fill=\"black\" />" << std::endl;
        }
        file << "</svg>" << std::endl;
        file.close();
    }

    void PatternLayout::saveToLayoutEditorMacro(std::string filename) {
        if (filename == "") {
            filename = classname + ".svg";
        }

        std::ofstream file;
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw Exception("Error creating file.");
        }
        file.precision(15);
        file << "#!/usr/bin/layout" << std::endl;
        file << "#name=" << filename << std::endl;
        file << "#help=This macro was generated with Vernier library." << std::endl;
        file << std::endl;
        file << "int main() {" << std::endl;
        std::vector<Rectangle> rectangleList;
        toRectangleVector(rectangleList);
        for (int i = 0; i < rectangleList.size(); i++) {
            file << "layout->drawing->point(" << 1000 * (rectangleList[i].x + margin) << "," << -1000 * (rectangleList[i].y + margin) << ");" << std::endl;
            file << "layout->drawing->point(" << 1000 * (rectangleList[i].x + margin + rectangleList[i].width) << "," << -1000 * (rectangleList[i].y + margin + rectangleList[i].height) << ");" << std::endl;
            file << "layout->drawing->box();" << std::endl;
        }
        file << "}" << std::endl;
        file.close();
    }

    void PatternLayout::saveToCSV(std::string filename) {
        if (filename == "") {
            filename = classname + ".csv";
        }

        std::ofstream file;
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw Exception("Error creating file.");
        }
        file.precision(15);
        file << "x;y;width;height:intensity" << std::endl;
        std::vector<Rectangle> rectangleList;
        toRectangleVector(rectangleList);
        for (int i = 0; i < rectangleList.size(); i++) {
            file << rectangleList[i].x << ";";
            file << rectangleList[i].y << ";";
            file << rectangleList[i].width << ";";
            file << rectangleList[i].height << ";";
            file << "1" << std::endl;
        }
        file.close();
    }

    void PatternLayout::saveToJSON(std::string filename) {
        if (filename == "") {
            filename = classname + ".json";
        }

        std::ofstream file;
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw Exception("Error creating file.");
        }

        file.precision(15);
        file.setf(std::ios::fixed, std::ios::floatfield);
        file << "{" << std::endl;

        writeJSON(file);

        file << "        \"copyright\": \"Copyright (c) 2018-2022 UBFC, ENSMM, UFC, CNRS.\"" << std::endl;
        file << "    }" << std::endl;
        file << "}" << std::endl;
        file.close();
    }

    void PatternLayout::writeJSON(std::ofstream & file) {
        file << "    \"" << classname << "\": {" << std::endl;
        file << "        \"description\": \"" << description << "\"," << std::endl;
        file << "        \"date\": \"" << date << "\"," << std::endl;
        file << "        \"author\": \"" << author << "\"," << std::endl;
        file << "        \"unit\": \"" << unit << "\"," << std::endl;
        file << "        \"margin\": " << margin << "," << std::endl;
    }

    void PatternLayout::readJSON(rapidjson::Value & document) {
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
        if (document.HasMember("margin") && document["margin"].IsDouble()) {
            margin = document["margin"].GetDouble();
        } else {
            margin = 0.0;
        }
    };

    void PatternLayout::loadFromJSON(std::string filename) {
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
    };

    void PatternLayout::renderOrthographicProjection(Pose pose, Eigen::ArrayXXd & outputImage) {
        if (outputImage.rows() <= 0 || outputImage.rows() % 2 == 1) {
            throw Exception("The number of rows must be positive and even.");
        }
        if (outputImage.cols() <= 0 || outputImage.cols() % 2 == 1) {
            throw Exception("The number of columns must be positive and even.");
        }

        double cx = outputImage.cols() / 2.0;
        double cy = outputImage.rows() / 2.0;

        Eigen::MatrixXd cameraMatrix(3, 4);
        cameraMatrix << 1 / pose.pixelSize, 0, 0, cx,
                0, 1 / pose.pixelSize, 0, cy,
                0, 0, 0, 1;

        Eigen::Matrix4d cTp = pose.getCameraToPatternTransformationMatrix();

        Eigen::MatrixXd M(4, 3);
        M << 1, 0, 0,
                0, 1, 0,
                0, 0, 0,
                0, 0, 1;

        Eigen::Matrix3d projectionMatrix = cameraMatrix * cTp * M;
        Eigen::Matrix3d inverseMatrix = projectionMatrix.inverse();

        for (int col = 0; col < outputImage.cols(); col++) {
            for (int row = 0; row < outputImage.rows(); row++) {
                Eigen::Vector3d pointImage(col + 0.5, row + 0.5, 1);
                Eigen::Vector3d pointPattern = inverseMatrix * pointImage;
                outputImage(row, col) = this->getIntensity(pointPattern.x(), pointPattern.y());
            }
        }
    }

    void PatternLayout::renderPerspectiveProjection(Pose pose, double focalLength, Eigen::ArrayXXd & outputImage) {

        double cx = outputImage.cols() / 2.0;
        double cy = outputImage.rows() / 2.0;

        Eigen::MatrixXd cameraMatrix(3, 4);
        cameraMatrix << focalLength / pose.pixelSize, 0, cx, 0,
                0, focalLength / pose.pixelSize, cy, 0,
                0, 0, 1, 0;

        Eigen::Matrix4d cTp = pose.getCameraToPatternTransformationMatrix();

        Eigen::MatrixXd M(4, 3);
        M << 1, 0, 0,
                0, 1, 0,
                0, 0, 0,
                0, 0, 1;

        Eigen::Matrix3d projectionMatrix = cameraMatrix * cTp * M;
        Eigen::Matrix3d inverseMatrix = projectionMatrix.inverse();

        for (int col = 0; col < outputImage.cols(); col++) {
            for (int row = 0; row < outputImage.rows(); row++) {
                Eigen::Vector3d pointImage(col + 0.5, row + 0.5, 1);
                Eigen::Vector3d pointPattern = inverseMatrix * pointImage;
                pointPattern /= pointPattern.z();
                outputImage(row, col) = this->getIntensity(pointPattern.x(), pointPattern.y());
            }
        }
    }

    void PatternLayout::saveToPNG(std::string filename) {
        throw Exception("saveToPNG is not implemented for " + this->classname);
    }

}