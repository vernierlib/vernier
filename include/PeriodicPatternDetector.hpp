/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef PERIODICPATTERNDETECTOR_HPP
#define PERIODICPATTERNDETECTOR_HPP

#include "PatternDetector.hpp"
#include "PatternPhase.hpp"

namespace vernier {

    /** \brief Class to estimate the pose of periodic patterns with subpixel resolutions.  
     */
    class PeriodicPatternDetector : public PatternDetector {
    protected:
        double physicalPeriod;
        PatternPhase patternPhase;
        PhasePlane plane1, plane2;
        int periodShift1, periodShift2;
        int betaSign, gammaSign;
        bool computePhaseGradient;

        void readJSON(const rapidjson::Value& document) override;
        
        void computeImage() override;
    
    public:

        /** Constructs a detector for periodic patterns
         *
         * \param physicalPeriod: physical period of the pattern
         */
        PeriodicPatternDetector(double physicalPeriod = 1.0);
       
        /** Returns true of a periodic pattern has been found */
        bool patternFound(int id = -1) override;
        
        int patternCount() override;

        /** Returns the 2D pose of the pattern (assuming an orthographic projection) */
        Pose get2DPose(int id = -1) override;

        /** Returns the most likely 3D pose of the pattern (assuming a perspective 
         * projection with a long-focus lens) 
         *
         * The phase gradient mode must be actived before image computing.
         */
        Pose get3DPose(int id = -1) override;
        
        /** Returns the four 3D possible poses of the pattern (assuming an 
         * orthographic projection) */
        std::vector<Pose> getAll3DPoses(int id = -1) override;
        
        void showControlImages() override;
        
        void draw(cv::Mat& image) override;
        
        std::string toString() override;

        /** Returns the reference to the pattern phase class */
        PatternPhase * getPatternPhase() {
            return &patternPhase;
        }

        /** Tells the detector to estimate the most likely pose with phase gradients */
        void setPhaseGradientMode(bool value = true);

        /** Returns true if the phase gradient mode is activated */
        bool isPhaseGradientMode();

        /** Sets the approximate length of one period in pixels */
        void setPixelPeriod(double pixelPeriod);

        /** Sets the physical period of the pattern */
        void setPhysicalPeriod(double physicalPeriod);

        /** Sets the width of the gaussian filter */
        void setSigma(double sigma);

        /** Sets the ratio of pixels to crop from the border for the regression */
        void setCropFactor(double cropFactor);

        /** Returns the phase plane corresponding to the first direction of the pattern */
        PhasePlane getPlane1();

        /** Returns the phase plane corresponding to the second direction of the pattern */
        PhasePlane getPlane2();

        Eigen::ArrayXXd getUnwrappedPhase1();

        Eigen::ArrayXXd getUnwrappedPhase2();

        void setDouble(const std::string & attribute, double value) override;

        void setBool(const std::string & attribute, bool value) override;

        double getDouble(const std::string & attribute) override;

        bool getBool(const std::string & attribute) override;

        void* getObject(const std::string & attribute) override;

    };
}

#endif