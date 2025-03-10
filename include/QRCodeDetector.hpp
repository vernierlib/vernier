/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef QRCODEDETECTOR_HPP
#define QRCODEDETECTOR_HPP

#include "QRFiducialDetector.hpp"

namespace vernier {
    
    class QRCode {
    public:

        cv::Point2d top, right, bottom, center;

        /** Constructs a default QRCode */
        QRCode() {
        };

        /** Constructs a QRCode based on three fiducial patterns */
        QRCode(QRFiducialPattern& marker0, QRFiducialPattern& marker1, QRFiducialPattern& marker2);

        /** Draws the QRCode in an image */
        void draw(cv::Mat& image);

        /** Converts into a string */
        std::string toString();

        double getRadius();

        double getAngle();
    };

    class QRCodeDetector {
    private:

        std::vector<std::vector<double> > distance;
        std::vector<std::vector<int> > clusters;

        void computeDistances();
        void makeFirstClustering();
        void recordCodes();

    public:

        QRFiducialDetector fiducialDetector;

        /** Vector of detected codes (unsort) */
        std::vector<QRCode> codes;

        /** Constructs a detector for QR codes */
        QRCodeDetector() : fiducialDetector() {
        };

        /** Detects QR codes in an image */
        void compute(const cv::Mat& image);

        /** Draws the found codes in an image (detection must have been done before) */
        void draw(cv::Mat& image);

        /** Converts into a string */
        std::string toString();
    };

}

#endif