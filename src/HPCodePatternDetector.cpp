/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "HPCodePatternDetector.hpp"

namespace vernier {

    HPCodePatternDetector::HPCodePatternDetector(double physicalPeriod, int numberHalfPeriods, int snapshotSize)
    : PeriodicPatternDetector(physicalPeriod) {
        classname = "HPCodePattern";
        resize(physicalPeriod, numberHalfPeriods, snapshotSize);
    }

    void HPCodePatternDetector::resize(double physicalPeriod, int numberHalfPeriods, int snapshotSize) {
        ASSERT_MSG(physicalPeriod > 0.0 , "The period must be positive.");
        ASSERT_MSG((numberHalfPeriods > 0) && (numberHalfPeriods % 4 == 1), "The size s of the HPCode must be s = 4k+1.");
        this->snapshotSize = snapshotSize;
        this->physicalPeriod = physicalPeriod;
        this->numberHalfPeriods = numberHalfPeriods;
        snapshot.resize(snapshotSize, snapshotSize);
        patternPhase.resize(snapshotSize, snapshotSize);
        window = hannWindow(snapshotSize, 1);
    }

    void HPCodePatternDetector::readJSON(const rapidjson::Value& document) {
        throw Exception("HPCodePatternDetector::readJSON is not implemented yet.");
    }

    void HPCodePatternDetector::computeImage() {

        detector.compute(image8U);

        markers.clear();
        for (int i = 0; i < detector.codes.size(); i++) {

            QRCode code = detector.codes[i];
            
            int diameter = (int) (2 * code.getRadius());
            if (snapshot.cols() < diameter) {
                std::cout << "The HPCode is too large for pose estimation: increase the snapshot size over " << diameter << " pixels." << std::endl;
            }
            if (2 * diameter < numberHalfPeriods) {
                std::cout << "The HPCode is too tiny for pose estimation: increase the picture quality size." << std::endl;
            }

            int centerX = (int) code.center.x;
            int centerY = (int) code.center.y;
            
            takeSnapshot(centerX, centerY, snapshotSize, array, snapshot);
            patternPhase.compute(snapshot * window);       
            plane1 = patternPhase.getPlane1();
            plane2 = patternPhase.getPlane2();

            if (patternPhase.peaksFound()) {

                double alpha;
                double dx, dy;
                double diffAngle = angleInPiPi(plane1.getAngle() - code.getAngle());
                if (diffAngle >= -PI / 4 && diffAngle <= PI / 4) {
                    alpha = plane1.getAngle();
                    dx = -plane1.getPosition(physicalPeriod, 0.0, 0.0);
                    dy = -plane2.getPosition(physicalPeriod, 0.0, 0.0);
                } else if (diffAngle >= 3 * PI / 4 || diffAngle <= -3 * PI / 4) {
                    alpha = plane1.getAngle() + PI;
                    dx = +plane1.getPosition(physicalPeriod, 0.0, 0.0);
                    dy = +plane2.getPosition(physicalPeriod, 0.0, 0.0);
                } else if (diffAngle >= PI / 4 && diffAngle <= 3 * PI / 4) {
                    alpha = plane1.getAngle() - PI / 2;
                    dx = plane2.getPosition(physicalPeriod, 0.0, 0.0);
                    dy = -plane1.getPosition(physicalPeriod, 0.0, 0.0);
                } else {
                    alpha = plane1.getAngle() + PI / 2;
                    dx = -plane2.getPosition(physicalPeriod, 0.0, 0.0);
                    dy = plane1.getPosition(physicalPeriod, 0.0, 0.0);
                }

                double pixelSize = physicalPeriod / patternPhase.getPixelPeriod();
                double xImg = (centerX - image8U.cols / 2);
                double yImg = (centerY - image8U.rows / 2);
                double x = pixelSize * (xImg * cos(alpha) - yImg * sin(-alpha)) + dx;
                double y = pixelSize * (xImg * sin(-alpha) + yImg * cos(alpha)) + dy;

                Pose pose = Pose(x, y, alpha, pixelSize);

                if (numberHalfPeriods % 4 == 1) {
                    unsigned long id = readNumber(code, image8U, patternPhase.getPixelPeriod() / 2.0);
                    markers.insert(std::make_pair(id, pose));
                } else {
                    markers.insert(std::make_pair(i, pose));
                }
            }
        }
    }

    unsigned long HPCodePatternDetector::readNumber(QRCode& code, const cv::Mat& image, double dotSize) {
        cv::Point2d rightDirection = (code.right - code.top);
        rightDirection *= dotSize / cv::norm(rightDirection);
        cv::Point2d upDirection = (code.top - code.bottom);
        upDirection *= dotSize / cv::norm(upDirection);
        cv::Point2d end = code.top + 3 * upDirection + 3.5 * rightDirection;
        cv::Point2d start = code.right + 3 * upDirection - 3.5 * rightDirection;

        // Getting the line
        cv::LineIterator it(image, cv::Point2i(start), cv::Point2i(end), 8);
        cv::Mat rasterLine(1, it.count, CV_8U);
        for (int i = 0; i < it.count; i++, ++it) {
            rasterLine.at<unsigned char>(0, i) = **it;
        }

        // Analysing the line
        int dotCount = 7 + (int) std::round((cv::norm(code.top - code.right) / dotSize));
        //std::cout << "dotCount : " << 7 + (double)(cv::norm(code.top - code.right) / dotSize) << std::endl;
        cv::Mat dataLine(1, dotCount - 14, CV_8U);
        cv::resize(rasterLine, dataLine, dataLine.size(), 0, 0, cv::INTER_AREA);
        cv::threshold(dataLine, dataLine, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        //std::cout << "data line : " << dataLine << std::endl;
        int zeroCount = 0;
        for (int col = 0; col < dataLine.cols; col += 2) {
            if (dataLine.at<unsigned char>(0, col) == 0) {
                zeroCount++;
            }
        }
        int lowLevel = 0;
        if (zeroCount < dataLine.cols / 4) {
            lowLevel = 255;
        }

        // Reading the data line
        unsigned long number = 0;
        int n = 0;
        for (int col = 1; col < dataLine.cols - 1; col += 2) {
            if (dataLine.at<unsigned char>(0, col) == lowLevel) {
                number |= 1 << n;
            }
            n++;
        }

        return number;
    }

    Pose HPCodePatternDetector::get2DPose(int id) {
        if (id < 0) {
            return markers.begin()->second;
        } else {
            return markers.at(id);
        }
    }

    bool HPCodePatternDetector::patternFound(int id) {
        if (id < 0) {
            return (markers.size() > 0);
        } else {
            return (markers.find(id) != markers.end());
        }
    }

    int HPCodePatternDetector::patternCount() {
        return markers.size();
    };

    Pose HPCodePatternDetector::get3DPose(int id) {
        throw Exception("HPCodePatternDetector::get3DPose is not implemented yet.");
        return Pose();
    }

    std::vector<Pose> HPCodePatternDetector::getAll3DPoses(int id) {
        throw Exception("HPCodePatternDetector::getAll3DPoses is not implemented yet.");
        return std::vector<Pose> ();
    }

    void HPCodePatternDetector::draw(cv::Mat & image) {
        PatternDetector::draw(image);
        for (std::map<int, Pose>::iterator it = markers.begin(); it != markers.end(); it++) {
            double length = 2 * patternPhase.getPixelPeriod() * this->numberHalfPeriods / 4;
            it->second.draw(image, length, to_string(it->first));
        }
    }

    //    void HPCodePatternDetector::drawSnapshots(cv::Mat & image) {
    //        for (std::map<int, Pose>::iterator it = codes.begin(); it != codes.end(); it++) {
    //            int x = (int) (it->second.x);
    //            int y = (int) (it->second.y);
    //            cv::rectangle(image, cv::Rect(x - snapshotSize / 2, y - snapshotSize / 2, snapshotSize, snapshotSize), cv::Scalar(255, 0, 0));
    //        }
    //    }

    void HPCodePatternDetector::showControlImages() {
        detector.showControlImages();
        //cv::imshow("Canny image", detector.fiducialDetector.cannyImage);
        patternPhase.showControlImages();
        //cv::imshow("Found peaks (red = dir 1, green = dir 2)", patternPhase.getPeaksImage());
        arrayShow("Snapshot image", snapshot);
    }

}