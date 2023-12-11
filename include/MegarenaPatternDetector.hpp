/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef MEGARENADETECTOR_HPP
#define MEGARENADETECTOR_HPP

#include "PeriodicPatternDetector.hpp"
#include "Thumbnail.hpp"
#include "AbsoluteDecoding.hpp"

namespace Vernier {

    /** \brief Class to estimate the absolute pose of periodic encoded patterns with subpixel resolutions.
     */
    class MegarenaPatternDetector : public PeriodicPatternDetector {
    protected:
        Eigen::ArrayXXi bitSequence;
        AbsoluteDecoding decoding;
        int length1, length2;
        Thumbnail thumbnail;
        int codePosition1, codePosition2;
        Plane plane1Save, plane2Save;

        void readJSON(rapidjson::Value& document);

        void computeAbsolutePose(Eigen::ArrayXXd& pattern);

    public:

        /** Constructs an empty detector for megarena periodic patterns */
        MegarenaPatternDetector();
        
        /** Constructs a detector for megarena patterns with a specific bit sequence
         *
         *	\param bitSequence: encoded sequence of the megarena pattern
         *	\param physicalPeriod: physical period of the pattern used to build it
         */
        MegarenaPatternDetector(Eigen::ArrayXXi bitSequence, double physicalPeriod = 1.0);
        
        /** Constructs a detector for megarena patterns with a specific code size
         *
         *	\param physicalPeriod: physical period of the pattern used to build it
         *	\param codeSize: size of the code
         */
        MegarenaPatternDetector(double physicalPeriod, int codeSize);

        ~MegarenaPatternDetector() = default;

        void compute(Eigen::ArrayXXd& pattern);

        Pose get2DPose();

        std::vector<Pose> getAll3DPoses();
        
        void showControlImages(int delay = -1);
        
        /** Returns the computed thumbnail of the image given to the megarena detector
         */
        Thumbnail getThumbnail();

        int getInt(const std::string & attribute);

        void* getObject(const std::string & attribute);

    };
}

#endif