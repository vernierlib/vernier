/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "BitmapPatternDetector.hpp"

namespace vernier {

    BitmapPatternDetector::BitmapPatternDetector()
    : PeriodicPatternDetector() {
        classname = "BitmapPattern";
    }

    BitmapPatternDetector::BitmapPatternDetector(double physicalPeriod, const std::string filename)
    : PeriodicPatternDetector(physicalPeriod) {
        classname = "BitmapPattern";
        description = "created from " + filename;
        bitmap.resize(4);
        cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        image.convertTo(bitmap[0], CV_8U);

        for (int k = 0; k < 3; k++) {
            cv::rotate(bitmap[k], bitmap[k + 1], cv::ROTATE_90_CLOCKWISE);
        }
    }

    void BitmapPatternDetector::readJSON(rapidjson::Value& document) {
        throw Exception("BitmapPatternDetector::readJSON is not implemented yet.");
    }

    void BitmapPatternDetector::compute(const Eigen::ArrayXXd& array) {
        PeriodicPatternDetector::compute(array);
        computeThumbnail(plane1, plane2, array, PI / 4);
        computeAbsolutePose(array);
    }

    void BitmapPatternDetector::computeAbsolutePose(const Eigen::ArrayXXd& array) {
        double maxmaxVal = -1;
        int maxAngle;
        for (int k = 0; k < 4; k++) {
            int angle = k * 90;
            cv::Mat result;
            cv::matchTemplate(thumbnail, bitmap[k], result, cv::TM_CCOEFF);

            double maxVal;
            cv::Point maxLoc;
            cv::minMaxLoc(result, NULL, &maxVal, NULL, &maxLoc, cv::Mat());

            if (maxVal > maxmaxVal) {
                maxmaxVal = maxVal;
                maxAngle = angle;
                periodShift1 = -(maxLoc.x - result.cols / 2) / 2;
                periodShift2 = -(maxLoc.y - result.rows / 2) / 2;
            }

            //            result.convertTo(result, CV_32F);
            //            cv::normalize(result, result, 1.0, 0, cv::NORM_MINMAX);
            //            cv::drawMarker(result, maxLoc, cv::Scalar(0, 0, 255));
            //            cv::imshow("Result", result);
            //            cv::waitKey(0);
        }

        if (maxAngle == 90) {
            std::swap(plane1, plane2);
            plane2.flip();
            std::swap(periodShift1, periodShift2);
            periodShift2 = -periodShift2;

        } else if (maxAngle == 180) {
            plane1.flip();
            plane2.flip();
            periodShift1 = -periodShift1;
            periodShift2 = -periodShift2;

        } else if (maxAngle == 270) {
            std::swap(plane1, plane2);
            plane1.flip();
            std::swap(periodShift1, periodShift2);
            periodShift1 = -periodShift1;
        }
    }

    void BitmapPatternDetector::computeThumbnail(Plane plane1, Plane plane2, const Eigen::ArrayXXd& array, double deltaPhase) {
        double approxPixelPeriod = (plane1.getPixelicPeriod() + plane2.getPixelicPeriod()) / 2.0;
        int length1 = (int) (2.82 * array.rows() / approxPixelPeriod);
        int length2 = (int) (2.82 * array.cols() / approxPixelPeriod);
        if (length1 % 2 == 0) length1++;
        if (length2 % 2 == 0) length2++;

        thumbnail = cv::Mat(length2, length1, CV_8U);

        Eigen::ArrayXXd numberWhiteDots(length1, length2);
        Eigen::ArrayXXd cumulWhiteDots(length1, length2);
        Eigen::ArrayXXd numberBackgroundDots(length1, length2);
        Eigen::ArrayXXd cumulBackgroundDots(length1, length2);

        numberWhiteDots.fill(0);
        cumulWhiteDots.fill(0);
        numberBackgroundDots.fill(0);
        cumulBackgroundDots.fill(0);

        int phaseIteration1, phaseIteration2;

        Eigen::Vector3d tempPlane1Coeff(plane1.getA(), plane1.getB(), plane1.getC());
        Eigen::Vector3d tempPlane2Coeff(plane2.getA(), plane2.getB(), plane2.getC());

        Plane tempPlane1(tempPlane1Coeff);
        Plane tempPlane2(tempPlane2Coeff);


        for (int col = 0; col < array.cols(); col++) {
            for (int row = 0; row < array.rows(); row++) {

                double phaseCol = tempPlane1.getPhase(row - array.rows() / 2, col - array.cols() / 2);
                double phaseRow = tempPlane2.getPhase(row - array.rows() / 2, col - array.cols() / 2);

                phaseIteration1 = round(phaseCol / PI) + length1 / 2;
                phaseIteration2 = round(phaseRow / PI) + length2 / 2;

                if (phaseIteration1 < cumulBackgroundDots.rows() && phaseIteration2 < cumulBackgroundDots.cols() && phaseIteration1 >= 0 && phaseIteration2 >= 0) {
                    if ((abs(std::fmod(phaseCol, PI)) <= deltaPhase || abs(std::fmod(phaseCol, PI)) >= PI - deltaPhase) && (abs(std::fmod(phaseRow, PI)) <= deltaPhase || abs(std::fmod(phaseRow, PI)) >= PI - deltaPhase)) {
                        numberWhiteDots(phaseIteration1, phaseIteration2) += 1;
                        cumulWhiteDots(phaseIteration1, phaseIteration2) += array(row, col);
                    }
                }
            }
        }

        for (int col = 0; col < thumbnail.cols; col++) {
            for (int row = 0; row < thumbnail.rows; row++) {
                thumbnail.at<char>(row, col) = (char) (255 * cumulWhiteDots(col, row) / numberWhiteDots(col, row));
            }
        }
    }

    Pose BitmapPatternDetector::get2DPose(int id) {
        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, periodShift1);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, periodShift2);
        double alpha = plane1.getAngle();
        double pixelSize = physicalPeriod / plane1.getPixelicPeriod();

        return Pose(x, y, alpha, pixelSize);
    }

    Pose BitmapPatternDetector::get3DPose(int id) {
        Pose pose = getAll3DPoses()[0];
        pose.beta *= betaSign;
        pose.gamma *= gammaSign;
        return pose;
    }

    std::vector<Pose> BitmapPatternDetector::getAll3DPoses(int id) {
        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, periodShift1);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, periodShift2);

        std::vector<Pose> poseVec = PeriodicPatternDetector::getAll3DPoses();
        for (int i = 0; i < poseVec.size(); i++) {
            poseVec[i].x = x;
            poseVec[i].y = y;
        }
        return poseVec;
    }

    cv::Mat BitmapPatternDetector::getThumbnail() {
        return thumbnail;
    }

    void BitmapPatternDetector::showControlImages(int delay) {
        PeriodicPatternDetector::showControlImages();
        cv::Mat zoom;
        cv::resize(thumbnail, zoom, cv::Size(thumbnail.rows * THUMBNAIL_ZOOM, thumbnail.cols * THUMBNAIL_ZOOM), THUMBNAIL_ZOOM, THUMBNAIL_ZOOM, cv::INTER_NEAREST);
        cv::imshow("Thumbnail", zoom);
    }

    int BitmapPatternDetector::getInt(const std::string & attribute) {
        if (attribute == "periodShift1") {
            return periodShift1;
        } else if (attribute == "periodShift2") {
            return periodShift2;
        } else {
            return PeriodicPatternDetector::getInt(attribute);
        }
    }

    void* BitmapPatternDetector::getObject(const std::string & attribute) {
        if (attribute == "bitmap") {
            return &bitmap;
        } else if (attribute == "thumbnail") {
            return &thumbnail;
        } else {
            return PeriodicPatternDetector::getObject(attribute);
        }
    }

}