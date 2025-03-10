/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef MEGARENADETECTOR_HPP
#define MEGARENADETECTOR_HPP

#include "PeriodicPatternDetector.hpp"
#include "MegarenaThumbnail.hpp"
#include "MegarenaAbsoluteDecoding.hpp"

namespace vernier {

    /** \brief Class to estimate the absolute pose of periodic encoded patterns with subpixel resolutions.
     */
    class MegarenaPatternDetector : public PeriodicPatternDetector {
    protected:
        
        Eigen::ArrayXXi bitSequence;
        MegarenaAbsoluteDecoding decoding;
        MegarenaThumbnail thumbnail;

        void readJSON(rapidjson::Value& document) override;

        void computeAbsolutePose(const Eigen::ArrayXXd& pattern);

    public:

        /** Constructs an empty detector for megarena periodic patterns */
        MegarenaPatternDetector();
        
        /** Constructs a detector for megarena patterns with a specific bit sequence
         *
         *	\param physicalPeriod: physical period of the pattern used to build it
         *	\param bitSequence: encoded sequence of the megarena pattern
         */
        MegarenaPatternDetector(double physicalPeriod, Eigen::ArrayXXi bitSequence);
        
        /** Constructs a detector for megarena patterns with a specific code size
         *
         *	\param physicalPeriod: physical period of the pattern used to build it
         *	\param codeSize: size of the code
         */
        MegarenaPatternDetector(double physicalPeriod, int codeSize);

        ~MegarenaPatternDetector() = default;

        void computeArray(const Eigen::ArrayXXd& pattern) override;

        void showControlImages(int delay = -1) override;
        
        /** Returns the computed thumbnail of the image given to the megarena detector
         */
        MegarenaThumbnail getThumbnail();

        int getInt(const std::string & attribute) override;

        void* getObject(const std::string & attribute) override;

    };
}

#endif