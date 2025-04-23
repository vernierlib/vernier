/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef BITMAPPATTERNDETECTOR_HPP
#define BITMAPPATTERNDETECTOR_HPP

#include "PeriodicPatternDetector.hpp"
#include "BitmapThumbnail.hpp"

namespace vernier {

    /** \brief Class to estimate the absolute pose of periodic encoded patterns with subpixel resolutions.
     */
    class BitmapPatternDetector : public PeriodicPatternDetector {
    protected:

        int maxAngle;
        BitmapThumbnail bitmapThumbnail;
        std::vector<cv::Mat> bitmap;
        int bitmapIndex;

        void readJSON(const rapidjson::Value& document) override;

        void computeAbsolutePose();

        void computeImage() override;

    public:

        /** Constructs an empty detector for bitmap patterns */
        BitmapPatternDetector();

        /** Constructs a detector for bitmap patterns with a specific image
         *
         *	\param physicalPeriod: physical period of the pattern used to build it
         *	\param filename: name of the bitmap file
         */
        BitmapPatternDetector(double physicalPeriod, const std::string & filename);

        void showControlImages() override;

        /** Returns the computed thumbnail of the image 
         */
        cv::Mat getThumbnail();

        int getInt(const std::string & attribute) override;

        void* getObject(const std::string & attribute) override;

    };
}

#endif