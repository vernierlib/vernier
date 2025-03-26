/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef PATTERNDETECTOR_HPP
#define PATTERNDETECTOR_HPP

#include "Common.hpp"

namespace vernier {

    /** Abstract class to define the interface of pattern detectors. 
     *  
     * \example detectingStampPattern.cpp     
     * \example detectingHPCodePattern.cpp 
     * \example detectingMegarenaPattern.cpp
     * \example detectingMegarenaPattern3D.cpp 
     */
    class PatternDetector {
    protected:
        
        std::string classname;
        cv::Mat image32F;
        cv::Mat image64F;
        cv::Mat image8U;
        Eigen::ArrayXXd array;

        virtual void readJSON(const rapidjson::Value& document);

        virtual void computeImage() = 0;

    public:

        /** modifiable variables */
        std::string description;
        std::string date;
        std::string author;
        std::string unit;

        /** Default constructor */
        PatternDetector();

        /** Initializes a pattern detector from a JSON file */
        void loadFromJSON(const std::string filename);

        /** Detects and estimates the poses of the patterns in the image stored in a OpenCV Mat. 
         * The image must be 8-bit or 32-bit floating-point. */
        void compute(const cv::Mat & image);

        /** Detects and estimates the poses of the patterns in an image stored in a double array          
         */
        void compute(const Eigen::ArrayXXd & array);

        /** Returns true if patterns have been detected and localized */
        virtual bool patternFound(int id = -1) = 0;
        
        /** Returns the number of detected patterns */
        virtual int patternCount() = 0;

        /** Returns the 2D pose of the pattern */
        virtual Pose get2DPose(int id = -1) = 0;

        /** Returns the most likely 3D pose of the pattern */
        virtual Pose get3DPose(int id = -1) = 0;

        /** Returns the possible 3D poses of the pattern in case of ambiguities */
        virtual std::vector<Pose> getAll3DPoses(int id = -1) = 0;

        /** Displays the images to check the pattern detector working. */
        virtual void showControlImages() = 0;

        /** Draws the found patterns in a image (analysis must have been done before) */
        virtual void draw(cv::Mat& image);

        /** Returns a string describing the detector */        
        virtual std::string toString();

        std::string getAuthor();

        std::string getDate();

        std::string getDescription();

        std::string getUnit();

        std::string getClassname();

        /** Returns the attribute address corresponding to the given name */
        virtual void* getObject(const std::string & attribute);

        /** Returns the attribute value corresponding to the given name */
        virtual double getDouble(const std::string & attribute);

        /** Returns the attribute value corresponding to the given name */
        virtual int getInt(const std::string & attribute);

        /** Returns the attribute value corresponding to the given name */
        virtual bool getBool(const std::string & attribute);

        /** Returns the attribute value corresponding to the given name */
        virtual std::string getString(const std::string & attribute);

        /** Sets the attribute value corresponding to the given name */
        virtual void setDouble(const std::string & attribute, double value);

        /** Sets the attribute value corresponding to the given name */
        virtual void setInt(const std::string & attribute, int value);

        /** Sets the attribute value corresponding to the given name */
        virtual void setBool(const std::string & attribute, bool value);

        /** Sets the attribute value corresponding to the given name */
        virtual void setString(const std::string & attribute, std::string value);

        friend class Detector;
    };

}

#endif