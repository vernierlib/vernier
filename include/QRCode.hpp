/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef QRCODE_HPP
#define QRCODE_HPP

#include "QRMarkerDetector.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    class QRCode {
    public:

        cv::Point2d top, right, bottom, center;

        /** Constructs a default QRCode */
        QRCode() {
        };

        /** Constructs a QRCode based on three markers */
        QRCode(QRMarker& marker0, QRMarker& marker1, QRMarker& marker2);

        /** Draws the code in an image */
        void draw(cv::Mat& image);


        /** Converts into a string */
        std::string toString();

        double getRadius();

        double getAngle();
    };
#endif // USE_OPENCV
}

#endif