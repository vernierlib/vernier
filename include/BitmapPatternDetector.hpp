/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef BITMAPPATTERNDETECTOR_HPP
#define BITMAPPATTERNDETECTOR_HPP

#include "PeriodicPatternDetector.hpp"

namespace vernier {

    /** \brief Class to estimate the absolute pose of periodic encoded patterns with subpixel resolutions.
     */
    class BitmapPatternDetector : public PeriodicPatternDetector {
    protected:
        int periodShift1, periodShift2;
        cv::Mat thumbnail;
        std::vector<cv::Mat> bitmap;

        void readJSON(rapidjson::Value& document);

        void computeAbsolutePose(const Eigen::ArrayXXd& pattern);
        
        void computeThumbnail(Plane plane1, Plane plane2, const Eigen::ArrayXXd& array, double deltaPhase);

    public:
        
        int THUMBNAIL_ZOOM = 5;

        /** Constructs an empty detector for bitmap patterns */
        BitmapPatternDetector();
        
        /** Constructs a detector for bitmap patterns with a specific image
         *
         *	\param physicalPeriod: physical period of the pattern used to build it
         *	\param filename: name if the image file
         */
        BitmapPatternDetector(double physicalPeriod, const std::string filename);
        
        void compute(const Eigen::ArrayXXd& array);

        Pose get2DPose(int id = 0);
        
        Pose get3DPose(int id = 0);

        std::vector<Pose> getAll3DPoses(int id = 0);
        
        void showControlImages(int delay = -1);
        
        /** Returns the computed thumbnail of the image 
         */ 
        cv::Mat getThumbnail();

        int getInt(const std::string & attribute);
        
        void* getObject(const std::string & attribute);

    };
}

#endif