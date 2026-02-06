/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "PatternLayout.hpp"

namespace vernier {

    PatternLayout::PatternLayout() {
        classname = "PatternLayout";
        description = "Layout generated with the Vernier library";
        date = currentDateTime();
        author = "FEMTO-ST";
        unit = "um";
        originX = 0.0;
        originY = 0.0;
        leftMargin = 0.0;
        rightMargin = 0.0;
        topMargin = 0.0;
        bottomMargin = 0.0;
        width = 0.0;
        height = 0.0;
    }

    void PatternLayout::saveToJSON(std::string filename) {
        if (filename == "") {
            filename = classname + ".json";
        }

        std::ofstream file;
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw Exception("Error creating file " + filename);
        }

        file.precision(15);
        file.setf(std::ios::fixed, std::ios::floatfield);
        file << "{" << std::endl;

        writeJSON(file);

        //file << "        \"copyright\": \"Creative Commons Attribution 4.0\"" << std::endl;
        if (author != "") {
        file << "        \"author\": \"" << author << "\"," << std::endl;
        }
        if (date != "") {
        file << "        \"date\": \"" << date << "\"" << std::endl;
        } else {
            file << "        \"date\": \"" << currentDateTime() << "\"" << std::endl;
        }
        file << "    }" << std::endl;
        file << "}" << std::endl;
        file.close();
    }

    void PatternLayout::writeJSON(std::ofstream & file) {
        file << "    \"" << classname << "\": {" << std::endl;
        if (description != "") {
            file << "        \"description\": \"" << description << "\"," << std::endl;
        }
        if (unit != "") {
        file << "        \"unit\": \"" << unit << "\"," << std::endl;
        }
        if (leftMargin > 0) {
            file << "        \"leftMargin\": " << to_string(leftMargin) << "," << std::endl;
        }
        if (rightMargin > 0) {
            file << "        \"rightMargin\": " << to_string(rightMargin) << "," << std::endl;
        }
        if (topMargin > 0) {
            file << "        \"topMargin\": " << to_string(topMargin) << "," << std::endl;
        }
        if (bottomMargin > 0) {
            file << "        \"bottomMargin\": " << to_string(bottomMargin) << "," << std::endl;
        }
    }

    void PatternLayout::readJSON(const rapidjson::Value & document) {
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
        if (document.HasMember("leftMargin") && document["leftMargin"].IsDouble()) {
            leftMargin = document["leftMargin"].GetDouble();
        } else {
            leftMargin = 0.0;
        }
        if (document.HasMember("rightMargin") && document["rightMargin"].IsDouble()) {
            rightMargin = document["rightMargin"].GetDouble();
        } else {
            rightMargin = 0.0;
        }
        if (document.HasMember("topMargin") && document["topMargin"].IsDouble()) {
            topMargin = document["topMargin"].GetDouble();
        } else {
            topMargin = 0.0;
        }
        if (document.HasMember("bottomMargin") && document["bottomMargin"].IsDouble()) {
            bottomMargin = document["bottomMargin"].GetDouble();
        } else {
            bottomMargin = 0.0;
        }
        if (document.HasMember("margin") && document["margin"].IsDouble()) {
            setMargins(document["margin"].GetDouble());
        }
    };

    void PatternLayout::loadFromJSON(const std::string & filename) {
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

    void PatternLayout::saveToSVG(std::string filename) {
        if (filename == "") {
            filename = classname + ".svg";
        }

        std::ofstream file;
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw Exception("Error creating file" + filename);
        }

        file.precision(15);
        file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
        file << "<!-- Created with the Vernier library -->" << std::endl;
        file << "<svg" << std::endl;
        file << "    xmlns=\"http://www.w3.org/2000/svg\"" << std::endl;
        file << "    version=\"1.1\"" << std::endl;
        file << "    x=\"0\" y=\"0\" width=\"" << width + leftMargin + rightMargin << "\" height=\"" << height + topMargin + bottomMargin << "\"" << std::endl;
        file << "    viewBox=\"0 0 " << width + leftMargin + rightMargin << " " << height + topMargin + bottomMargin << "\">" << std::endl;
        file << "<title>" << description << "</title>" << std::endl;
        file << "<desc>" << std::endl;
        file << "    class: " << classname << std::endl;
        file << "    description: " << description << std::endl;
        file << "    date: " << date << std::endl;
        file << "    author: " << author << std::endl;
        file << "    unit: " << unit << std::endl;
        file << "    width: " << width << std::endl;
        file << "    height: " << height << std::endl;
        file << "    originX: " << originX << std::endl;
        file << "    originY: " << originY << std::endl;
        file << "    leftMargin: " << leftMargin << std::endl;
        file << "    rightMargin: " << rightMargin << std::endl;
        file << "    topMargin: " << topMargin << std::endl;
        file << "    bottomMargin: " << bottomMargin << std::endl;
        file << "</desc>" << std::endl;
        std::vector<Rectangle> rectangleList;
        toRectangleVector(rectangleList);
        for (int i = 0; i < rectangleList.size(); i++) {
            file << "<rect x=\"" << rectangleList[i].x + leftMargin << "\" ";
            file << "y=\"" << rectangleList[i].y + topMargin << "\" ";
            file << "width=\"" << rectangleList[i].width << "\" ";
            file << "height=\"" << rectangleList[i].height << "\" ";
            file << "fill=\"black\" />" << std::endl;
            if (i % (rectangleList.size() / 100) == 0) {
                std::cout << " \r Writing " << filename << " : " << 100 * i / rectangleList.size() << " %            " << std::flush;
            }
        }
        file << "</svg>" << std::endl;
        file.close();
        std::cout << "\r Writing " << filename << " : completed            " << std::endl;
    }

    void PatternLayout::saveToLayoutEditorMacro(std::string filename) {
        if (filename == "") {
            filename = classname + ".svg";
        }

        std::ofstream file;
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw Exception("Error creating file" + filename);
        }
        file.precision(15);
        file << "#!/usr/bin/layout" << std::endl;
        file << "#name=" << filename << std::endl;
        file << "#help=This macro was generated with the Vernier library." << std::endl;
        file << std::endl;
        file << "int main() {" << std::endl;
        std::vector<Rectangle> rectangleList;
        toRectangleVector(rectangleList);
        for (int i = 0; i < rectangleList.size(); i++) {
            file << "layout->drawing->point(" << 1000 * (rectangleList[i].x + leftMargin) << "," << -1000 * (rectangleList[i].y + topMargin) << ");" << std::endl;
            file << "layout->drawing->point(" << 1000 * (rectangleList[i].x + leftMargin + rectangleList[i].width) << "," << -1000 * (rectangleList[i].y + topMargin + rectangleList[i].height) << ");" << std::endl;
            file << "layout->drawing->box();" << std::endl;
            if (i % (rectangleList.size() / 100) == 0) {
                std::cout << " \r Writing " << filename << " : " << 100 * i / rectangleList.size() << " %            " << std::flush;
            }
        }
        file << "}" << std::endl;
        file.close();
        std::cout << "\r Writing " << filename << " : completed            " << std::endl;
    }

#ifndef WIN32
    gdstk::Cell * PatternLayout::convertToGDSCell(std::string name) {
        if (name == "") {
            name = classname;
        }

        std::vector<vernier::Rectangle> rectangleList;
        toRectangleVector(rectangleList);

        gdstk::Cell * cell = new gdstk::Cell();
        cell->init(name.c_str());
        for (int i = 0; i < rectangleList.size(); i++) {
            gdstk::Polygon * polygon = new gdstk::Polygon(gdstk::rectangle(gdstk::Vec2{rectangleList[i].x + leftMargin, -(rectangleList[i].y + topMargin)}, gdstk::Vec2{rectangleList[i].x + rectangleList[i].width + leftMargin, -(rectangleList[i].y + rectangleList[i].height + topMargin)}, gdstk::make_tag(1, 1)));
            cell->polygon_array.append(polygon);
            if (i % (rectangleList.size() / 100) == 0) {
                std::cout << " \r Building cell " << name << " : " << 100 * i / rectangleList.size() << " %            " << std::flush;
            }
        }

        //        rectangleList.push_back(Rectangle(0.0, 0.0, leftMargin + width + rightMargin, topMargin));
        //        rectangleList.push_back(Rectangle(0.0, topMargin, leftMargin, height));
        //        rectangleList.push_back(Rectangle(leftMargin + width, topMargin, rightMargin, height));
        //        rectangleList.push_back(Rectangle(0.0, topMargin + height, leftMargin + width + rightMargin, bottomMargin));

        gdstk::Array<gdstk::Polygon*> all_text = {};
        gdstk::text(toString().c_str(), 8 * rectangleList[0].height, gdstk::Vec2{0, -(topMargin + height + bottomMargin + 8 * rectangleList[0].height)}, false, 2, all_text);
        cell->polygon_array.extend(all_text);

        return cell;
    }

    void PatternLayout::saveToGDS(std::string filename) {
        if (filename == "") {
            filename = classname + ".oas";
        }

        gdstk::Library lib = {};
        lib.init(classname.c_str(), 1e-6, 1e-9);
        gdstk::Cell * cell = convertToGDSCell();
        lib.cell_array.append(cell);

        std::cout << "\r Writing " << filename << " : starting...            " << std::flush;
        lib.write_gds(filename.c_str(), 0, NULL);
        //lib.write_oas(filename.c_str(), 0, 6, OASIS_CONFIG_DETECT_ALL);
        //cell->write_svg(filename.c_str(), 10, 6, NULL, NULL, "#222222", 5, true, NULL);

        lib.clear();
        cell->clear(); // possible memory leak: are polygons really deleted?
        delete cell;
        std::cout << "\r Writing " << filename << " : completed            " << std::endl;
    }

    void PatternLayout::saveToOASIS(std::string filename) {
        if (filename == "") {
            filename = classname + ".oas";
        }

        gdstk::Library lib = {};
        lib.init(classname.c_str(), 1e-6, 1e-9);
        gdstk::Cell * cell = convertToGDSCell();
        lib.cell_array.append(cell);

        std::cout << "\r Writing " << filename << " : starting...            " << std::flush;
        //lib.write_gds(filename.c_str(), 0, NULL);
        lib.write_oas(filename.c_str(), 0, 6, OASIS_CONFIG_DETECT_ALL);
        //cell->write_svg(filename.c_str(), 10, 6, NULL, NULL, "#222222", 5, true, NULL);

        lib.clear();
        cell->clear(); // possible memory leak: are polygons really deleted?
        delete cell;
        std::cout << "\r Writing " << filename << " : completed            " << std::endl;
    }
#endif

    void PatternLayout::saveToCSV(std::string filename) {
        if (filename == "") {
            filename = classname + ".csv";
        }

        std::ofstream file;
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw Exception("Error creating file " + filename);
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
    
    void PatternLayout::renderOrthographicProjection(Pose pose, cv::Mat & outputImage, Eigen::Vector2d principalPoint) {
        Eigen::ArrayXXd array(outputImage.rows, outputImage.cols);
        renderOrthographicProjection(pose, array, principalPoint);
        eigen2cv(array, outputImage);         
    }

    void PatternLayout::renderOrthographicProjection(Pose pose, Eigen::ArrayXXd & outputImage, Eigen::Vector2d principalPoint) {
        if (outputImage.rows() <= 0 || outputImage.rows() % 2 == 1) {
            throw Exception("The number of rows must be positive and even.");
        }
        if (outputImage.cols() <= 0 || outputImage.cols() % 2 == 1) {
            throw Exception("The number of columns must be positive and even.");
        }

        if (principalPoint(0) < 0) {
            principalPoint(0) = outputImage.cols() / 2.0;
            principalPoint(1) = outputImage.rows() / 2.0;
        }

        Eigen::MatrixXd cameraMatrix(3, 4);
        cameraMatrix << 1 / pose.pixelSize, 0, 0, principalPoint(0),
                0, 1 / pose.pixelSize, 0, principalPoint(1),
                0, 0, 0, 1;

        Eigen::Matrix4d cTp = pose.getCameraToPatternTransformationMatrix();

        Eigen::MatrixXd M(4, 3);
        M << 1, 0, 0,
                0, 1, 0,
                0, 0, 0,
                0, 0, 1;

        Eigen::Matrix3d homography = cameraMatrix * cTp * M;
        Eigen::Matrix3d inversedHomography = homography.inverse();

        for (int col = 0; col < outputImage.cols(); col++) {
            for (int row = 0; row < outputImage.rows(); row++) {
                //Eigen::Vector3d pointImage(col + 0.5, row + 0.5, 1);
                Eigen::Vector3d pointImage(col, row, 1);
                Eigen::Vector3d pointPattern = inversedHomography * pointImage;
                outputImage(row, col) = this->getIntensity(pointPattern.x(), pointPattern.y());
            }
        }
    }
    
    void PatternLayout::renderPerspectiveProjection(Pose pose, cv::Mat & outputImage, double focalLength, Eigen::Vector2d principalPoint) {
        Eigen::ArrayXXd array(outputImage.rows, outputImage.cols);
        renderPerspectiveProjection(pose, array, focalLength, principalPoint);
        eigen2cv(array, outputImage);         
    }

    void PatternLayout::renderPerspectiveProjection(Pose pose, Eigen::ArrayXXd & outputImage, double focalLength, Eigen::Vector2d principalPoint) {
        if (outputImage.rows() <= 0 || outputImage.rows() % 2 == 1) {
            throw Exception("The number of rows must be positive and even.");
        }
        if (outputImage.cols() <= 0 || outputImage.cols() % 2 == 1) {
            throw Exception("The number of columns must be positive and even.");
        }

        if (principalPoint(0) < 0) {
            principalPoint(0) = outputImage.cols() / 2.0;
            principalPoint(1) = outputImage.rows() / 2.0;
        }

        Eigen::MatrixXd cameraMatrix(3, 4);
        cameraMatrix << focalLength / pose.pixelSize, 0, principalPoint(0), 0,
                0, focalLength / pose.pixelSize, principalPoint(1), 0,
                0, 0, 1, 0;

        Eigen::Matrix4d cTp = pose.getCameraToPatternTransformationMatrix();

        Eigen::MatrixXd M(4, 3);
        M << 1, 0, 0,
                0, 1, 0,
                0, 0, 0,
                0, 0, 1;

        Eigen::Matrix3d homography = cameraMatrix * cTp * M;
        Eigen::Matrix3d inversedHomography = homography.inverse();

        for (int col = 0; col < outputImage.cols(); col++) {
            for (int row = 0; row < outputImage.rows(); row++) {
                //Eigen::Vector3d pointImage(col + 0.5, row + 0.5, 1);
                Eigen::Vector3d pointImage(col, row, 1);
                Eigen::Vector3d pointPattern = inversedHomography * pointImage;
                pointPattern /= pointPattern.z();
                outputImage(row, col) = this->getIntensity(pointPattern.x(), pointPattern.y());
            }
        }
    }

    std::string PatternLayout::toString() {
        return classname;
    }

    void PatternLayout::saveToPNG(std::string filename) {
        throw Exception("saveToPNG is not implemented for " + this->classname);
    }

    std::string PatternLayout::getClassname() {
        return classname;
    }

    double PatternLayout::getOriginX() {
        return originX;
    }

    double PatternLayout::getOriginY() {
        return originY;
    }

    double PatternLayout::getHeight() {
        return height;
    }

    double PatternLayout::getWidth() {
        return width;
    }

    void* PatternLayout::getObject(const std::string & attribute) {
        throw Exception("The parameter " + attribute + " is not accessible or defined in class " + classname + ".");
    }

    double PatternLayout::getDouble(const std::string & attribute) {
        if (attribute == "originX") {
            return originX;
        } else if (attribute == "originY") {
            return originY;
        } else if (attribute == "leftMargin") {
            return leftMargin;
        } else if (attribute == "rightMargin") {
            return rightMargin;
        } else if (attribute == "topMargin") {
            return topMargin;
        } else if (attribute == "bottomMargin") {
            return bottomMargin;
        } else if (attribute == "width") {
            return width;
        } else if (attribute == "height") {
            return height;
        } else {
            throw Exception("The parameter " + attribute + " is not accessible or defined in class " + classname + ".");
        }
    }

    int PatternLayout::getInt(const std::string & attribute) {
        throw Exception("The parameter " + attribute + " is not accessible or defined in class " + classname + ".");
    }

    bool PatternLayout::getBool(const std::string & attribute) {
        throw Exception("The parameter " + attribute + " is not accessible or defined in class " + classname + ".");
    }

    void PatternLayout::setMargins(double margins) {
        leftMargin = margins;
        rightMargin = margins;
        topMargin = margins;
        bottomMargin = margins;
    }

    std::string PatternLayout::getString(const std::string & attribute) {
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
            throw Exception("The parameter " + attribute + " is not accessible or defined in class " + classname + ".");
        }
    }

    void PatternLayout::setDouble(const std::string & attribute, double value) {
        if (attribute == "leftMargin") {
            leftMargin = value;
        } else if (attribute == "rightMargin") {
            rightMargin = value;
        } else if (attribute == "topMargin") {
            topMargin = value;
        } else if (attribute == "bottomMargin") {
            bottomMargin = value;
        } else if (attribute == "margins") {
            setMargins(value);
        } else {
            std::cout << "The parameter " + attribute + " is not accessible or defined in class " + classname + "." << std::endl;
        }
    }

    void PatternLayout::setInt(const std::string & attribute, int value) {
        std::cout << "The parameter " + attribute + " is not accessible or defined in class " + classname + "." << std::endl;
    }

    void PatternLayout::setBool(const std::string & attribute, bool value) {
        std::cout << "The parameter " + attribute + " is not accessible or defined in class " + classname + "." << std::endl;
    }

    void PatternLayout::setString(const std::string & attribute, std::string value) {
        if (attribute == "classname") {
            classname = value;
        } else if (attribute == "description") {
            description = value;
        } else if (attribute == "date") {
            date = value;
        } else if (attribute == "author") {
            author = value;
        } else if (attribute == "unit") {
            unit = value;
        } else {
            std::cout << "The parameter " + attribute + " is not accessible or defined in class " + classname + "." << std::endl;
        }
    }


}