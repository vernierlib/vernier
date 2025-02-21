/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef HPCODEPATTERNDETECTOR_HPP
#define HPCODEPATTERNDETECTOR_HPP

#include "QRCodeDetector.hpp"
#include "PatternPhase.hpp"
#include <map>

namespace vernier {

    class HPCodePatternDetector {
    protected:

        Eigen::ArrayXXcd snapshot;
        int numberHalfPeriods;
        double physicalPeriod;
        int snapshotSize;

        void takeSnapshot(int x, int y, cv::Mat image);

        unsigned long readNumber(QRCode& code, cv::Mat& image, double dotSize);

    public:

        QRCodeDetector detector;
        PatternPhase patternPhase;

        /** Map of detected codes (key map = code id) */
        std::map<int, Pose> codes;

        /** Constructs a pose estimator for periodic QR codes
         *
         *	\param physicalPeriod: physical period between the dots of the QR code
         *	\param snapshotSize: maximal size of the QR code in pixels
         *	\param numberHalfPeriods: number of half periods contained in the QR along one direction.
         */
        HPCodePatternDetector(double physicalPeriod = 1.0, int snapshotSize = 128, int numberHalfPeriods = 37);

        /** Prepares the different required objects for processing
         *
         *	\param physicalPeriod: physical period between the dots of the QR code
         *	\param snapshotSize: maximal size of the QR code in pixels
         *	\param numberHalfPeriods: number of half periods contained in the QR along one direction.
         */
        void resize(double physicalPeriod, int snapshotSize, int numberHalfPeriods);

        /** Estimate the pose of a HP code in a image
         *
         *	\param QRCode: code
         *	\param image: image
         */
        void compute(cv::Mat& image);


        /** Draws all the found patterns in a image (analysis must have been done before)
         *
         *	\param image: image of any types
         */
        void drawPose(cv::Mat& image);
        
        void drawSnapshot(); // a supprimer

        double getPixelSize(); // a supprimer

        void showControlImages();

    };

}

#endif