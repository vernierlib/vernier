/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "MegarenaPatternDetector.hpp"
#include "MegarenaBitSequence.hpp"

namespace vernier {

    MegarenaPatternDetector::MegarenaPatternDetector()
    : PeriodicPatternDetector() {
        classname = "MegarenaPattern";
    }

    MegarenaPatternDetector::MegarenaPatternDetector(double physicalPeriod, Eigen::ArrayXXi bitSequence)
    : PeriodicPatternDetector(physicalPeriod) {
        if (bitSequence.rows() != 1) {
            throw Exception("The bit sequence must have a single row");
        }
        if (bitSequence.cols() <= 0) {
            throw Exception("The bit sequence must have at least one column.");
        }
        classname = "MegarenaPattern";
        this->bitSequence = bitSequence;
        decoding.resize(bitSequence);
    }

    MegarenaPatternDetector::MegarenaPatternDetector(double physicalPeriod, int codeSize)
    : PeriodicPatternDetector(physicalPeriod) {
        MegarenaBitSequence::generate(codeSize, bitSequence);
        classname = "MegarenaPattern";
        decoding.resize(bitSequence);
    }

    void MegarenaPatternDetector::computeArray(const Eigen::ArrayXXd& pattern) {
        resize(pattern.rows(), pattern.cols());
        PeriodicPatternDetector::computeArray(pattern);
        computeAbsolutePose(pattern);
    }

    void MegarenaPatternDetector::computeAbsolutePose(const Eigen::ArrayXXd& pattern) {
        double approxPixelPeriod = (plane1.getPixelicPeriod() + plane2.getPixelicPeriod()) / 2.0;

        int length1 = (pattern.rows() / (approxPixelPeriod));
        int length2 = (pattern.cols() / (approxPixelPeriod));

        length1++;
        length2++;

        if (length1 % 2 == 0) {
            length1++;
        }
        if (length2 % 2 == 0) {
            length2++;
        }

        thumbnail.resize(length1, length2);
        thumbnail.compute(plane1, plane2, pattern);

        Eigen::ArrayXXd sequence1 = thumbnail.getSequence1();
        Eigen::ArrayXXd sequence2 = thumbnail.getSequence2();

        int MSB1 = thumbnail.getMSB1();
        int MSB2 = thumbnail.getMSB2();

        periodShift1 = decoding.findCodePosition(sequence1, MSB1);
        periodShift2 = decoding.findCodePosition(sequence2, MSB2);

        //        plane1Save = plane1;
        //        plane2Save = plane2;

        if (periodShift1 >= 0 && periodShift2 >= 0) {
            // no change	
            //std::cout<<"code1>0 && code2>0"<<std::endl;
        } else if (periodShift1 < 0 && periodShift2 >= 0) {
            std::swap(periodShift1, periodShift2);
            std::swap(plane1, plane2);
            plane2.flip();
            //std::cout<<"code1<0 && code2>0"<<std::endl;
            thumbnail.rotate270();
        } else if (periodShift1 >= 0 && periodShift2 < 0) {
            std::swap(periodShift1, periodShift2);
            std::swap(plane1, plane2);
            plane1.flip();
            //std::cout<<"code1>0 && code2<0"<<std::endl;
            thumbnail.rotate90();
        } else {
            plane1.flip();
            plane2.flip();
            //std::cout<<"code1<0 && code2<0"<<std::endl;
            thumbnail.rotate180();
        }

        periodShift1 = std::abs(periodShift1);
        periodShift2 = std::abs(periodShift2);

    }

    MegarenaThumbnail MegarenaPatternDetector::getThumbnail() {
        return thumbnail;
    }

    void MegarenaPatternDetector::showControlImages() {
        cv::imshow("Thumbnail", this->thumbnail.getMeanDotsImage());
        //cv::moveWindow("Thumbnail", patternPhase.getNCols()*2, 0);
        PeriodicPatternDetector::showControlImages();
    }

    void MegarenaPatternDetector::draw(cv::Mat& image) {
        PatternDetector::draw(image);

        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, 0);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, 0);
        double alpha = plane1.getAngle();
        double pixelSize = physicalPeriod / plane1.getPixelicPeriod();
        Pose(x, y, alpha, pixelSize).draw(image);

        x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, periodShift1);
        y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, periodShift2);

        cv::putText(image, "x: " + to_string(x) +
                ", y: " + to_string(y) +
                ", rz: " + to_string(alpha) +
                ", px: " + to_string(pixelSize),
                cv::Point(3, 35), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 2);
    }

    void MegarenaPatternDetector::readJSON(rapidjson::Value& document) {

        PatternDetector::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            physicalPeriod = document["period"].GetDouble();
            PeriodicPatternDetector::setPhysicalPeriod(physicalPeriod);
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the period is missing or has a wrong format.");
        }

        if (document.HasMember("bitSequence") && document["bitSequence"].IsArray()) {
            bitSequence.resize(1, document["bitSequence"].Size());

            for (rapidjson::SizeType row = 0; row < bitSequence.cols(); row++) {
                const rapidjson::Value& value = document["bitSequence"][row];
                if (value.IsInt()) {
                    bitSequence(0, row) = value.GetInt();
                } else {
                    throw Exception("The file is not a valid bitmap pattern file, the row " + to_string(row) + " of the bitmap has a wrong format");
                }
            }
        } else if (document.HasMember("codeSize") && document["codeSize"].IsInt()) {
            int codeSize = document["codeSize"].GetInt();
            if (codeSize >= 4 && codeSize <= 12) {
                MegarenaBitSequence::generate(codeSize, bitSequence);
            } else {
                throw Exception("The file is not a valid megarena pattern file, the code size must between 4 and 12.");
            }
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the bitmap is missing or has a wrong format.");
        }

        decoding.resize(bitSequence);
    }

    std::string MegarenaPatternDetector::toString() {
        return PeriodicPatternDetector::toString() + ", codeSize: " + to_string(MegarenaBitSequence::codeDepth(bitSequence.cols())) + "bits";
    }

    int MegarenaPatternDetector::getInt(const std::string & attribute) {
        if (attribute == "codePosition1") {
            return periodShift1;
        } else if (attribute == "codePosition2") {
            return periodShift2;
        } else {
            return PeriodicPatternDetector::getInt(attribute);
        }
    }

    void* MegarenaPatternDetector::getObject(const std::string & attribute) {
        if (attribute == "bitSequence") {
            return &bitSequence;
        } else if (attribute == "decoding") {
            return &decoding;
        } else if (attribute == "thumbnail") {
            return &thumbnail;
        } else {
            return PeriodicPatternDetector::getObject(attribute);
        }
    }

}