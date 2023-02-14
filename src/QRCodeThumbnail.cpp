/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "QRCodeThumbnail.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    QRCodeThumbnail::QRCodeThumbnail() {
    }

    void QRCodeThumbnail::resize(int nRows, int nCols, int snapSize, int flagQRCode) {
        if (flagQRCode == QRCODE_33P) {
            qrCodeBackground.resize(17, 17);
            qrCodeBackground.setZero();
            qrCodeBackground.block(0, 0, 4, 4).setOnes();
            qrCodeBackground.block(13, 0, 4, 4).setOnes();
            qrCodeBackground.block(0, 13, 4, 4).setOnes();

            qrCodeForeground.resize(17, 17);
            qrCodeForeground.setOnes();
        } else if (flagQRCode == QRCODE_37P) {
            qrCodeBackground.resize(19, 19);
            qrCodeBackground.setZero();
            qrCodeBackground.block(0, 0, 4, 4).setOnes();
            qrCodeBackground.block(15, 0, 4, 4).setOnes();
            qrCodeBackground.block(0, 15, 4, 4).setOnes();

            qrCodeForeground.resize(19, 19);
            qrCodeForeground.setOnes();
        } else if (flagQRCode == QRCODE_39P) {
            qrCodeBackground.resize(20, 20);
            qrCodeBackground.setZero();
            qrCodeBackground.block(0, 0, 4, 4).setOnes();
            qrCodeBackground.block(16, 0, 4, 4).setOnes();
            qrCodeBackground.block(0, 16, 4, 4).setOnes();

            qrCodeForeground.resize(20, 20);
            qrCodeForeground.setOnes();
        }

        this->nRows = nRows;
        this->nCols = nCols;

        this->size = snapSize;
        this->sideOffset = (int) snapSize / 4;

        snapPhase.setSigma(0.005*size);
        snapPhase.resize(size, size);

        snapshot.resize(size, size);
    }

    void QRCodeThumbnail::compute(Eigen::ArrayXXd image, Plane& plane1, Plane& plane2) {
        this->pixelPeriod = plane1.getPixelicPeriod();
        this->length1 = nRows / (pixelPeriod - 1);
        this->length2 = nCols / (pixelPeriod - 1);

        length1++;
        length2++;

        if (length1 % 2 == 0) {
            length1++;
        }
        if (length2 % 2 == 0) {
            length2++;
        }

        thumbnail.resize(length1, length2);
        thumbnail.computeThumbnail(plane1, plane2, image);

        thumbnailArray = thumbnail.getThumbnailArray();
        removeNanFromArray(thumbnailArray);

        cv::Mat thumbnailMat(thumbnailArray.rows(), thumbnailArray.cols(), CV_64FC1, thumbnailArray.data());
        cv::Mat thumbnailMat32(thumbnailArray.rows(), thumbnailArray.cols(), CV_32FC1);
        thumbnailMat.convertTo(thumbnailMat32, CV_32FC1);
        cv::Mat qrCodeForegroundMat(qrCodeForeground.rows(), qrCodeForeground.cols(), CV_64FC1, qrCodeForeground.data());
        cv::Mat qrCodeForegroundMat32(qrCodeForeground.rows(), qrCodeForeground.cols(), CV_32FC1);
        qrCodeForegroundMat.convertTo(qrCodeForegroundMat32, CV_32FC1);

        cv::Mat result(thumbnailMat32.cols - qrCodeForegroundMat32.cols + 1, thumbnailMat32.rows - qrCodeForegroundMat32.rows + 1, CV_32FC1);
        cv::matchTemplate(thumbnailMat32, qrCodeForegroundMat32, result, 3);
        cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        double minVal;
        double maxVal;
        cv::Point minLoc;
        cv::Point maxLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
        int centerRow = maxLoc.y + qrCodeForegroundMat.rows / 2;
        int centerCol = maxLoc.x + qrCodeForegroundMat.cols / 2;

        int quarter = detectQuarter(maxLoc.x, maxLoc.y);

        double phi1 = (centerCol - length2 / 2) * 2 * PI;
        double phi2 = (centerRow - length1 / 2) * 2 * PI;

        double a1 = plane1.getA();
        double b1 = plane1.getB();
        double c1 = plane1.getC();
        double a2 = plane2.getA();
        double b2 = plane2.getB();
        double c2 = plane2.getC();

        this->centerY = round((phi2 * a1 - a2 * phi1 + a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1));
        this->centerX = round((phi1 - b1 * centerY - c1) / a1);

        double radius = pixelPeriod * std::max(qrCodeForegroundMat.rows, qrCodeForegroundMat.cols) / 2;

        takeSnapshot(centerX, centerY, radius, image);

        //cv::Mat snap(snapshot.rows(), snapshot.cols(), CV_64FC1, snapshot.data());
        //cv::imshow("snapshot", snap);

        Eigen::ArrayXXcd snapShotComplex(size, size);
        snapShotComplex.real() = snapshot;

        snapPhase.setPixelPeriod(pixelPeriod);
        snapPhase.compute(snapShotComplex);
        plane1 = snapPhase.getPlane1();
        plane2 = snapPhase.getPlane2();

        plane1.setC(plane1.getA() * (-sideOffset) + plane1.getB() * (-sideOffset) + plane1.getC());
        plane2.setC(plane2.getA() * (-sideOffset) + plane2.getB() * (-sideOffset) + plane2.getC());

        if (quarter == 0) {
            std::swap(plane1, plane2);
        }
        if (quarter == 1) {
            plane1.revertCoefficients();
        }
        if (quarter == 2) {
            std::swap(plane1, plane2);
            plane1.revertCoefficients();
            plane2.revertCoefficients();
        }
        if (quarter == 3) {
            plane2.revertCoefficients();
        }

        this->plane1 = plane1;
        this->plane2 = plane2;
    }

    int QRCodeThumbnail::getQRCodeCenterX() {
        return centerX;
    }

    int QRCodeThumbnail::getQRCodeCenterY() {
        return centerY;
    }

    void QRCodeThumbnail::takeSnapshot(int x, int y, int radius, Eigen::ArrayXXcd image) {
        for (int col = -radius; col < radius; col++) {
            for (int row = -radius; row < radius; row++) {
                double distanceToCenter = sqrt((row + 0.5) * (row + 0.5) + (col + 0.5) * (col + 0.5));
                double hanning = (1 + cos(PI * distanceToCenter / radius)) / 2;
                if (distanceToCenter < radius && y + row > 0 && x + row > 0 && x + row < image.rows() && y + row < image.cols()) {
                    snapshot(size / 2 + row, size / 2 + col) = hanning * image(y + row, x + col).real();
                }
            }
        }
    }

    void QRCodeThumbnail::removeNanFromArray(Eigen::ArrayXXd& image) {
        for (int i = 0; i < image.rows(); i++) {
            for (int j = 0; j < image.cols(); j++) {
                if (isnan(image(i, j))) {
                    image(i, j) = 0;
                }
            }
        }
    }

    int QRCodeThumbnail::detectQuarter(int rowStart, int colStart) {
        Eigen::ArrayXXd thumbnailBackground = thumbnail.getCumulBackground().block(rowStart, colStart, qrCodeBackground.rows(), qrCodeBackground.cols()) / thumbnail.getNumberBackground().block(rowStart, colStart, qrCodeBackground.rows(), qrCodeBackground.cols());

        cv::Mat thumbnailBackgroundMat(thumbnailBackground.rows(), thumbnailBackground.cols(), CV_64FC1, thumbnailBackground.data());
        cv::normalize(thumbnailBackgroundMat, thumbnailBackgroundMat, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        cv::Mat qrCodeBackgourndMat(qrCodeBackground.rows(), qrCodeBackground.cols(), CV_64FC1, qrCodeBackground.data());
        cv::Mat qrCodeBackgourndMat64(qrCodeBackgourndMat.rows, qrCodeBackgourndMat.cols, CV_64FC1);
        qrCodeBackgourndMat.convertTo(qrCodeBackgourndMat64, CV_64FC1);
        cv::normalize(qrCodeBackgourndMat, qrCodeBackgourndMat, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        std::vector<double> maxMatch;

        cv::Mat result = qrCodeBackgourndMat64 - thumbnailBackgroundMat;

        cv::Scalar meanValue;
        cv::meanStdDev(result, result, meanValue);
        maxMatch.push_back(meanValue.val[0]);

        cv::Mat rotationMat = cv::getRotationMatrix2D(cv::Point2f(qrCodeBackgourndMat64.cols / 2, qrCodeBackgourndMat64.rows / 2), -90, 1);
        cv::warpAffine(qrCodeBackgourndMat64, qrCodeBackgourndMat64, rotationMat, cv::Size(qrCodeBackgourndMat64.cols, qrCodeBackgourndMat64.rows));

        result.setTo(0);
        result = qrCodeBackgourndMat64 - thumbnailBackgroundMat;

        cv::meanStdDev(result, result, meanValue);
        maxMatch.push_back(meanValue.val[0]);

        rotationMat = cv::getRotationMatrix2D(cv::Point2f(qrCodeBackgourndMat64.cols / 2, qrCodeBackgourndMat64.rows / 2), -90, 1);
        cv::warpAffine(qrCodeBackgourndMat64, qrCodeBackgourndMat64, rotationMat, cv::Size(qrCodeBackgourndMat64.cols, qrCodeBackgourndMat64.rows));

        result.setTo(0);
        result = qrCodeBackgourndMat64 - thumbnailBackgroundMat;

        cv::meanStdDev(result, result, meanValue);
        maxMatch.push_back(meanValue.val[0]);

        rotationMat = cv::getRotationMatrix2D(cv::Point2f(qrCodeBackgourndMat64.cols / 2, qrCodeBackgourndMat64.rows / 2), -90, 1);
        cv::warpAffine(qrCodeBackgourndMat64, qrCodeBackgourndMat64, rotationMat, cv::Size(qrCodeBackgourndMat64.cols, qrCodeBackgourndMat64.rows));

        result.setTo(0);
        result = qrCodeBackgourndMat64 - thumbnailBackgroundMat;

        cv::meanStdDev(result, result, meanValue);
        maxMatch.push_back(meanValue.val[0]);

        //std::cout << maxMatch[0] << " " << maxMatch[1] << " " << maxMatch[2] << " " << maxMatch[3] << std::endl;

        //std::cout << "QUARTER : " << std::distance(maxMatch.begin(), std::min_element(maxMatch.begin(), maxMatch.end())) << std::endl;

        int quarter = std::distance(maxMatch.begin(), std::min_element(maxMatch.begin(), maxMatch.end()));

        return quarter;
    }
#endif //USE_OPENCV
}