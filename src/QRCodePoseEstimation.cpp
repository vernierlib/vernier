/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "QRCodePoseEstimation.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    QRCodePoseEstimation::QRCodePoseEstimation() {
    }

    QRCodePoseEstimation::QRCodePoseEstimation(double physicalPeriod, int size, int numberHalfPeriods) {
        resize(size, physicalPeriod, numberHalfPeriods);
    }

    void QRCodePoseEstimation::setPhysicalPeriod(double physicalPeriod) {
        this->physicalPeriod = physicalPeriod;
    }

    void QRCodePoseEstimation::setNumberHalfPeriods(int numberHalfPeriods) {
        this->numberHalfPeriods = numberHalfPeriods;
    }

    void QRCodePoseEstimation::resize(int size, double physicalPeriod, int numberHalfPeriods) {
        this->size = size;
        this->physicalPeriod = physicalPeriod;
        this->numberHalfPeriods = numberHalfPeriods;
        snapshot.resize(size, size);
        patternPhase.resize(size, size);
        this->pixelPeriodMin = 4;
        this->pixelPeriodMax = 100;
    }

    void QRCodePoseEstimation::takeSnapshot(int x, int y, int radius, cv::Mat image) {
        snapshot.real().setConstant(0);
        if (image.channels() > 1) {
            cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
        }

        cv::Mat snap64f;
        image.convertTo(snap64f, CV_64FC1);
        snap64f /= 256;

        for (int col = -radius; col < radius; col++) {
            for (int row = -radius; row < radius; row++) {
                double distanceToCenter = sqrt((row + 0.5) * (row + 0.5) + (col + 0.5) * (col + 0.5));
                double hanning = (1 + cos(PI * distanceToCenter / radius)) / 2;
                if (distanceToCenter < radius && y + row > 0 && x + row > 0 && y + row < snap64f.rows && x + row < snap64f.cols) {
                    snapshot.real()(size / 2 + row, size / 2 + col) = hanning * snap64f.at<double>(y + row, x + col);
                }
            }
        }
    }

    void QRCodePoseEstimation::compute(cv::Mat& image) {
        //ASSERT(image.type()==CV_64F);

        detector.compute(image);
        for (int i = 0; i < detector.codes.size(); i++) {
            QRCode code = detector.codes[i];

            if ((int) code.getRadius() > size / 2) {
                throw Exception("The QRCode is too large for pose estimation: increase the snapshot size.");
            }
            if ((int) code.getRadius() < 37) {
                throw Exception("The QRCode is too tiny for pose estimation: increase the picture quality size.");
            }

            double xCenter = code.getCenter().x;
            double yCenter = code.getCenter().y;

            double x = xCenter;
            double y = yCenter;

            takeSnapshot(x, y, (int) code.getRadius(), image);

            Pose pose;
            pose.is3D = false;

            patternPhase.setPixelPeriod(29);
            patternPhase.compute(snapshot);

            //patternPhase.computeQRCode(snapshot);

            Plane plane1 = patternPhase.getPlane1();
            Plane plane2 = patternPhase.getPlane2();

            //cv::namedWindow("spectrum", cv::WINDOW_FREERATIO);
            //cv::imshow("spectrum" + std::to_string(i), Spectrum::draw(PeriodicPatternPositioning.spectrumShifted, PeriodicPatternPositioning.mainPeak1, PeriodicPatternPositioning.mainPeak2));

            double periodSize = plane1.getPixelicPeriod();

            if (physicalPeriod != 0) {
                this->pixelSize = physicalPeriod / periodSize;

                pose.x = xCenter*pixelSize;
                pose.y = yCenter*pixelSize;
            } else {
                this->pixelSize = 1;

                pose.x = code.getCenter().x * pixelSize;
                pose.y = code.getCenter().y * pixelSize;
            }

            pose.alpha = plane1.getAngle();

            //std::cout << "pose number " << i << "  at" << pose.toString() << std::endl;

            if (numberHalfPeriods == 37) {
                unsigned long number = readNumber(code, image, periodSize / 2.0);

                codes.insert(std::make_pair(number, pose));
            } else {
                codes[0] = pose;
            }
        }
    }

    unsigned long QRCodePoseEstimation::readNumber(QRCode& code, cv::Mat& image, double dotSize) {
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

    void QRCodePoseEstimation::drawPose(cv::Mat& image) {
        detector.draw(image);

        for (std::map<int, Pose>::iterator it = codes.begin(); it != codes.end(); it++) {
            cv::line(image, cv::Point(it->second.x / pixelSize - 5, it->second.y / pixelSize), cv::Point(it->second.x / pixelSize + 5, it->second.y / pixelSize), cv::Scalar(0, 255, 0), 2);
            cv::line(image, cv::Point(it->second.x / pixelSize, it->second.y / pixelSize - 5), cv::Point(it->second.x / pixelSize, it->second.y / pixelSize + 5), cv::Scalar(0, 255, 0), 2);
            cv::putText(image, std::to_string(it->first), cv::Point(it->second.x / pixelSize + 5, it->second.y / pixelSize - 5), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255), 3);
            //it->second.draw(image);
        }
    }

    void QRCodePoseEstimation::drawSnapshot() {
        for (std::map<int, Pose>::iterator it = codes.begin(); it != codes.end(); it++) {
            cv::Mat snap64f;
            Eigen::MatrixXd snapMatrix;
            snapMatrix = snapshot.real().matrix();
            cv::eigen2cv(snapMatrix, snap64f);
            cv::imshow("snapshot " + std::to_string(it->first), snap64f);
            snap64f *= 255;
            cv::imwrite("snapshot.png", snap64f);
        }
    }

    double QRCodePoseEstimation::getPixelSize() {
        return pixelSize;
    }
#endif //USE_OPENCV
}