/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "QRCode.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    QRCode::QRCode(QRMarker& marker0, QRMarker& marker1, QRMarker& marker2) {
        // Determining the top marker
        double d01 = cv::norm(marker0.position - marker1.position);
        double d12 = cv::norm(marker1.position - marker2.position);
        double d02 = cv::norm(marker0.position - marker2.position);

        if (d01 > d02 && d01 > d12) {
            top = marker2.position;
            right = marker0.position;
            bottom = marker1.position;
        } else if (d12 > d01 && d12 > d02) {
            top = marker0.position;
            right = marker1.position;
            bottom = marker2.position;
        } else {
            top = marker1.position;
            right = marker0.position;
            bottom = marker2.position;
        }

        // Determining the positive sense of rotation
        cv::Point2d rightDirection = right - top;
        if (rightDirection.cross(bottom - top) < 0) {
            std::swap(right, bottom);
            rightDirection = right - top;
        }
    }

    double QRCode::getAngle() {
        cv::Point2d rightDirection = right - top;
        return atan2(rightDirection.y, rightDirection.x);
    }

    cv::Point2d QRCode::getCenter() {
        this->center = cv::Point2d((right.x + bottom.x) / 2.0, (right.y + bottom.y) / 2.0);
        return center;
    }

    void QRCode::draw(cv::Mat& image) {
        cv::line(image, top, right, cv::Scalar(0, 0, 255), 2);
        cv::line(image, top, bottom, cv::Scalar(0, 255, 0), 2);
        //cv::putText(image, Vernier::toString(number), center, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 4);
    }

    std::string QRCode::toString() {
        return "[ t=(" + Vernier::toString(top.x) + "," + Vernier::toString(top.y)
                + "), r=(" + Vernier::toString(right.x) + "," + Vernier::toString(right.y)
                + "), b=(" + Vernier::toString(bottom.x) + "," + Vernier::toString(bottom.y) + " ]";
    }

    double QRCode::getRadius() {
        int dotCount = (int) (cv::norm(top - right));
        return dotCount;
    }

    double QRCode::getPeriod() {
        return (cv::norm(top - right) / 15);
    }
#endif
}