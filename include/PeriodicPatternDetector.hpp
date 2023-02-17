/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef PERIODICPATTERNDETECTOR_HPP
#define PERIODICPATTERNDETECTOR_HPP

#include "PatternDetector.hpp"
#include "PatternPhase.hpp"

namespace Vernier {

    /** \brief Class to estimate the pose of periodic patterns with subpixel resolutions.  
     */
    class PeriodicPatternDetector : public PatternDetector {
    protected:
        double physicalPeriod;
        PatternPhase patternPhase;
        Plane plane1, plane2;
        int betaSign, gammaSign;

        void readJSON(rapidjson::Value& document);

    public:

        /** Constructs a detector for periodic patterns
         *
         * \param physicalPeriod: physical period of the pattern
         * \param approxPixelPeriod: approximate length of one period in pixels
         */
        PeriodicPatternDetector(double physicalPeriod = 1.0);

        ~PeriodicPatternDetector() = default;

        void resize(int nRows, int nCols);

        void compute(Eigen::ArrayXXd & array);

        Pose get2DPose();

        Pose get3DPose();

        std::vector<Pose> getAll3DPoses();

        /** Returns the 3D pose of the pattern (assuming a perspective projection with a pin-hole camera model) */
        Pose get3DPosePerspective(double focalLength, Eigen::Vector2d principalPoint);

        /** return the reference to the pattern phase class */
        PatternPhase * getPatternPhase() {
            return &patternPhase;
        }

        /** Sets the approximate length of one period in pixels */
        void setPixelPeriod(double pixelPeriod);

        /** Sets the physical period of the pattern */
        void setPhysicalPeriod(double physicalPeriod);

        /** Sets the width of the gaussian filter */
        void setSigma(double sigma);

        /** Sets the ratio of pixels to crop from the border for the regression */
        void setCropFactor(double cropFactor);

        /** Returns the phase plane corresponding to the first direction of the pattern */
        Plane getPlane1();

        /** Returns the phase plane corresponding to the second direction of the pattern */
        Plane getPlane2();

        void showControlImages();

        Eigen::ArrayXXd getUnwrappedPhase1();

        Eigen::ArrayXXd getUnwrappedPhase2();

        void setDouble(const std::string & attribute, double value);

        double getDouble(const std::string & attribute);

        void* getObject(const std::string & attribute);

    };
}

#endif