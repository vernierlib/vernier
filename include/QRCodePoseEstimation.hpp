/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef QRCODEPOSEESTIMATION_HPP
#define QRCODEPOSEESTIMATION_HPP

#include "QRCodeDetector.hpp"
#include "PatternPhase.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    class QRCodePoseEstimation {
    private:
        int nRows, nCols, sideOffset;
        double sigmaPercent, hyperGaussianOrder;
        double pixelPeriodMin, pixelPeriodMax;
        double pixelicPeriod;
        PatternPhase patternPhase;
        Eigen::ArrayXXcd snapshot;

    protected:
        double physicalPeriod;
        int size;

        void takeSnapshot(int x, int y, int radius, cv::Mat image);


    public:
        QRCodeDetector detector;
        double pixelSize;
        int numberHalfPeriods;

        /** Map of detected codes (key map = code number) */
        std::map<int, Pose> codes;

        //QRCodePoseEstimation();

        QRCodePoseEstimation();

        /** Constructs a pose estimator for periodic QR codes
         *
         *	\param physicalPeriod: physical period of the dots contained in the QR code
         *	\param size: maximal size of the QR code in pixels
         *	\param numberHalfPeriods: number of hlaf periods contained in the QR along one direction. New (numbered) QR codes contain 37 half periods while old contain 37.
         */
        QRCodePoseEstimation(double physicalPeriod, int size = 128, int numberHalfPeriods = 37);

        void setPhysicalPeriod(double physicalPeriod);

        void setNumberHalfPeriods(int numberHalfPeriods);

        /** Prepares the different required objects for processing
         *
         *	\param size: approximate size of the QR code in pixels
         */
        void resize(int size, double physicalPeriod, int numberHalfPeriods);

        /** Estimate the pose of a QR code in a image
         *
         *	\param QRCode: code
         *	\param image: image
         */
        void compute(cv::Mat& image);

        unsigned long readNumber(QRCode& code, cv::Mat& image, double dotSize);

        /** Draws all the found patterns in a image (analysis must have been done before)
         *
         *	\param image: image of any types
         */
        void drawPose(cv::Mat& image);

        void drawSnapshot();

        double getPixelSize();

    };
#endif // USE_OPENCV
}

#endif /* PERIODICPATTERNREGISTERING_HPP */