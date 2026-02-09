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

        /** Returns the 2D pose of the pattern assuming an orthographic projection 
         *
         * The method is described in the paper: André, A., Sandoz, P., Mauzé, B., Jacquot, M., & Laurent, G. (2020). Sensing one nanometer over ten centimeters: A microencoded target for visual in-plane position measurement. IEEE/ASME Transactions on Mechatronics, 25(3), 1193-1201.
         */
        Pose get2DPose(int id = -1) override;

        /** Returns the most likely 3D pose of the pattern (assuming a perspective 
         * projection with a long-focus lens) 
         *
         * The phase gradient mode must be actived before image computing.
         * 
         * OBSOLETE
         */
        Pose get3DPose(int id = -1) override;
        
        /** Returns the four 3D possible poses of the pattern assuming an 
         * orthographic projection. 
         * 
         * The method is described in the paper: André, A. N., Sandoz, P., Jacquot, M., & Laurent, G. J. (2022). Pose measurement at small scale by spectral analysis of periodic patterns. International Journal of Computer Vision, 130(6), 1566-1582.
         *
         */
        std::vector<Pose> getAll3DPoses(int id = -1) override;
        
        /** Returns the pose of the pattern assuming a perspective projection.
         *   
         * The method is described in the paper: André, A. N., Sandoz, P., Jacquot, M., & Laurent, G. J. (2022). Pose measurement at small scale by spectral analysis of periodic patterns. International Journal of Computer Vision, 130(6), 1566-1582.
         *
         * The phase gradient mode must be actived before image computing.
         */
        Pose get3DPosePerspective(double focalLength);
        
        /** Returns the pose of the pattern assuming a perspective projection 
         * rvec is the estimated rotation vector and tvec the estimated translation vector */
        void get3DPosePerspective(const cv::Mat & cameraMatrix, const cv::Mat & distortionCoefficients, cv::Mat & rvec, cv::Mat & tvec);
        
        void showControlImages() override;
        
        void draw(cv::Mat& image) override;
        
        std::string toString() override;

        /** Returns the reference to the pattern phase class */
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