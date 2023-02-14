/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef QRCODEDETECTOR_HPP
#define QRCODEDETECTOR_HPP

#include "QRMarkerDetector.hpp"
#include "QRCode.hpp"
#include <map>

namespace Vernier {
#ifdef USE_OPENCV

    class QRCodeDetector {
    private:

        std::vector<std::vector<double> > distance;
        std::vector<std::vector<int> > clusters;

        void computeDistances();
        void makeFirstClustering();
        void recordCodes();

    public:

        QRMarkerDetector markerDetector;

        /** Vector of detected codes (unsort) */
        std::vector<QRCode> codes;

        /** Constructs a detector for QR codes */
        QRCodeDetector() : markerDetector() {
        };

        /** Detects QR codes in an image */
        void compute(cv::Mat& image);

        /** Draws the found codes in an image (detection must have been done before) */
        void draw(cv::Mat& image);

        /** Converts into a string */
        std::string toString();
    };
#endif // USE_OPENCV
}

#endif