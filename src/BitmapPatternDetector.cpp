/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "BitmapPatternDetector.hpp"
#include "BitmapThumbnail.hpp"

namespace vernier {

    BitmapPatternDetector::BitmapPatternDetector()
    : PeriodicPatternDetector() {
        classname = "BitmapPattern";
    }

    BitmapPatternDetector::BitmapPatternDetector(double physicalPeriod, const std::string & filename)
    : PeriodicPatternDetector(physicalPeriod) {
        classname = "BitmapPattern";
        description = "created from " + filename;
        bitmap.resize(4);
        cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        ASSERT_MSG(!image.empty(), "The file " + filename + " is not found or is not an image.")
        image.convertTo(bitmap[0], CV_8U);

        for (int k = 0; k < 3; k++) {
            cv::rotate(bitmap[k], bitmap[k + 1], cv::ROTATE_90_CLOCKWISE);
        }
    }

    void BitmapPatternDetector::readJSON(const rapidjson::Value & document) {
        throw Exception("BitmapPatternDetector::readJSON is not implemented yet.");
    }

    void BitmapPatternDetector::computeImage() {
        PeriodicPatternDetector::computeImage();
        
        double approxPixelPeriod = (plane1.getPixelicPeriod() + plane2.getPixelicPeriod()) / 2.0;
        int length1 = (int) (2.82 * array.rows() / approxPixelPeriod);
        int length2 = (int) (2.82 * array.cols() / approxPixelPeriod);
        if (length1 % 2 == 0) length1++;
        if (length2 % 2 == 0) length2++;
        bitmapThumbnail.resize(std::max(length1, length2));
        bitmapThumbnail.compute(array, patternPhase.getPlane1(), patternPhase.getPlane2());
        
        computeAbsolutePose();
    }

    void BitmapPatternDetector::computeAbsolutePose() {
        double maxmaxVal = -1;
        for (int k = 0; k < 4; k++) {
            int angle = k * 90;
            cv::Mat result;
            //SHOW(bitmapThumbnail.thumbnail);
            //SHOW(bitmap[k]);
            //cv::waitKey(0);
            cv::matchTemplate(bitmapThumbnail.thumbnail, bitmap[k], result, cv::TM_CCOEFF);

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

    cv::Mat BitmapPatternDetector::getThumbnail() {
        return bitmapThumbnail.thumbnail;
    }

    void BitmapPatternDetector::showControlImages() {
        PeriodicPatternDetector::showControlImages();
        bitmapThumbnail.showControlImages();
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
            return &(bitmapThumbnail.thumbnail);
        } else {
            return PeriodicPatternDetector::getObject(attribute);
        }
    }

}