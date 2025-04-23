/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "BitmapThumbnail.hpp"
//#ifndef WIN32 
//#include "opencv2/img_hash.hpp"
//#endif

namespace vernier {

    BitmapThumbnail::BitmapThumbnail() {
    }

    BitmapThumbnail::BitmapThumbnail(int size) {
        resize(size);
    }

    void BitmapThumbnail::resize(int size) {
        ASSERT_MSG(size % 2 == 1, "The size of BitmapThumbnails must be odd");
        numberWhiteDots.resize(size, size);
        cumulWhiteDots.resize(size, size);
        thumbnail.create(size, size, CV_8U);
        binaryThumbnail.create(size, size, CV_8U);
    }

    void BitmapThumbnail::compute(const Eigen::ArrayXXd & array, PhasePlane plane1, PhasePlane plane2) {
        numberWhiteDots.fill(0);
        cumulWhiteDots.fill(0);

#pragma omp parallel for
        for (int col = 0; col < array.cols(); col++) {
            for (int row = 0; row < array.rows(); row++) {

                double phaseCol = plane1.getPhase(row - array.rows() / 2, col - array.cols() / 2);
                double phaseRow = plane2.getPhase(row - array.rows() / 2, col - array.cols() / 2);

                int phaseIteration1 = round(phaseCol / PI) + thumbnail.cols / 2;
                int phaseIteration2 = round(phaseRow / PI) + thumbnail.rows / 2;

                if (phaseIteration1 < numberWhiteDots.rows() && phaseIteration2 < numberWhiteDots.cols() && phaseIteration1 >= 0 && phaseIteration2 >= 0) {
                    if ((abs(std::fmod(phaseCol, PI)) <= DELTA_PHASE || abs(std::fmod(phaseCol, PI)) >= PI - DELTA_PHASE) && (abs(std::fmod(phaseRow, PI)) <= DELTA_PHASE || abs(std::fmod(phaseRow, PI)) >= PI - DELTA_PHASE)) {
                        numberWhiteDots(phaseIteration1, phaseIteration2) += 1;
                        cumulWhiteDots(phaseIteration1, phaseIteration2) += array(row, col);
                    }
                }
            }
        }

        double meanBackground = 0.0;
        int countBackground = 0;
        double meanForeground = 0.0;
        int countForeground = 0;
#pragma omp parallel for
        for (int row = 0; row < thumbnail.rows; row++) {
            for (int col = 0; col < thumbnail.cols; col++) {
                thumbnail.at<unsigned char>(row, col) = (unsigned char) (255 * cumulWhiteDots(col, row) / numberWhiteDots(col, row));
                if (row % 2 == 0 && col % 2 == 0) {
                    meanForeground += thumbnail.at<unsigned char>(row, col);
                    countForeground++;
                } else {
                    meanBackground += thumbnail.at<unsigned char>(row, col);
                    countBackground++;
                }
            }
        }
        meanBackground /= countBackground;
        meanForeground /= countForeground;

        unsigned char threshold = (unsigned char) ((meanBackground + meanForeground) / 2);
        cv::threshold(thumbnail, binaryThumbnail, threshold, 255, cv::THRESH_BINARY);

        //cv::threshold(thumbnail, binaryThumbnail, 127, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

        //cv::adaptiveThreshold(thumbnail, binaryThumbnail,255,cv::ADAPTIVE_THRESH_MEAN_C ,cv ::THRESH_BINARY, 5, 0);

        //        unsigned char highThreshold = (unsigned char) ((meanBackground + 2 * meanForeground) / 3);
        //        unsigned char lowThreshold = (unsigned char) ((2 * meanBackground + meanForeground) / 3);
        //#pragma omp parallel for
        //        for (int row = 0; row < thumbnail.rows; row++) {
        //            for (int col = 0; col < thumbnail.cols; col++) {
        //                binaryThumbnail.at<unsigned char>(row, col) = 0;
        //                if (row % 2 == 0 && col % 2 == 0) {
        //                    if (thumbnail.at<unsigned char>(row, col) > lowThreshold) {
        //                        binaryThumbnail.at<unsigned char>(row, col) = 255;
        //                    }
        //                } else {
        //                    if (thumbnail.at<unsigned char>(row, col) > highThreshold) {
        //                        binaryThumbnail.at<unsigned char>(row, col) = 255;
        //                    }
        //                }
        //            }
        //        }


    }

    //    cv::Point2d BitmapThumbnail::getCentroid() {
    //        cv::Moments mom = cv::moments(binaryThumbnail, false);
    //        return cv::Point2d(mom.m10 / mom.m00 - 0.5 * thumbnail.cols, mom.m01 / mom.m00 - 0.5 * thumbnail.rows);
    //    }

    //    int BitmapThumbnail::getCentroidDirection() {
    //        cv::Point2d direction = getCentroid();
    //        double angle = atan2(direction.y, direction.x);
    //        PRINT(angle)
    //        if (angle > -PI / 4 && angle < PI / 4)
    //            return 0;
    //        else if (angle > PI / 4 && angle < 3 * PI / 4)
    //            return 90;
    //        else if (angle > -3 * PI / 4 && angle < -PI / 4)
    //            return 270;
    //        else
    //            return 180;
    //    };
    //
    //    double BitmapThumbnail::getAngle() {
    //        cv::Point2d direction = getCentroid();
    //        double angle = atan2(direction.y, direction.x);
    //        return angle;
    //    };

    //    int BitmapThumbnail::hashCode(int angle) {
    //#ifndef WIN32         
    //        cv::Mat horizontalThumbnail;
    //        if (angle == 0) {
    //            binaryThumbnail.copyTo(horizontalThumbnail);
    //        } else if (angle == 90) {
    //            cv::rotate(binaryThumbnail, horizontalThumbnail, cv::ROTATE_90_COUNTERCLOCKWISE);
    //        } else if (angle == 180) {
    //            cv::rotate(binaryThumbnail, horizontalThumbnail, cv::ROTATE_180);
    //        } else {
    //            cv::rotate(binaryThumbnail, horizontalThumbnail, cv::ROTATE_90_CLOCKWISE);
    //        }
    //
    //        cv::Mat hash;
    //        cv::img_hash::averageHash(horizontalThumbnail, hash);
    //        return (int) hash.at<unsigned char>(0);
    //#else      
    //        return 0;
    //#endif
    //
    //    }

    //    void BitmapThumbnail::threshold() {
    //        double sumWhite=0;
    //        double countWhite=0;
    //        double sumBlack=0;
    //        double countBlack=0;
    //        for (int row = 0; row < thumbnail.rows; row++) {
    //            for (int col = 0; col < thumbnail.cols; col++) { 
    //                if (col%2 == 0 && row%2 == 0) {
    //                    countWhite++;
    //                    sumWhite += thumbnail.at<unsigned char>(row, col);
    //                } else {
    //                    countBlack++;
    //                    sumBlack += thumbnail.at<unsigned char>(row, col);
    //                }                  
    //            }
    //        } 
    //        double threshold = 255*(sumWhite/countWhite + sumBlack/countBlack)/2;
    //        cv::threshold(thumbnail, binaryThumbnail, threshold, 255, cv::THRESH_BINARY);    
    //    }

    void BitmapThumbnail::showControlImages() {
        cv::Mat zoom;
        cv::resize(binaryThumbnail, zoom, cv::Size(thumbnail.rows * THUMBNAIL_ZOOM, thumbnail.cols * THUMBNAIL_ZOOM), THUMBNAIL_ZOOM, THUMBNAIL_ZOOM, cv::INTER_NEAREST);
        cv::imshow("Binary Thumbnail", zoom);
        cv::resize(thumbnail, zoom, cv::Size(thumbnail.rows * THUMBNAIL_ZOOM, thumbnail.cols * THUMBNAIL_ZOOM), THUMBNAIL_ZOOM, THUMBNAIL_ZOOM, cv::INTER_NEAREST);
        cv::imshow("Thumbnail", zoom);
    }

}