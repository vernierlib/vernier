/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef PATTERNDETECTOR_HPP
#define PATTERNDETECTOR_HPP

#include "Common.hpp"

namespace Vernier {

    /** Abstract class to define the interface of pattern detectors. 
     * 
     * \example detectingMegarenaPattern2D.cpp
     * \example detectingMegarenaPattern3D.cpp  
     * \example detectingPeriodicPattern2D.cpp
     * \example detectingPeriodicPattern3D.cpp
     * \example detectingPeriodicPattern2DPerspective.cpp  
     */
    class PatternDetector {
    protected:
        std::string classname;
        std::string description;
        std::string date;
        std::string author;
        std::string unit;
        bool orthographicProjection;
        
        virtual void readJSON(rapidjson::Value& document);

        friend class Detector;

    public:

        /** Default constructor */
        PatternDetector();

        virtual ~PatternDetector() = default;

        /** Resizes the detector with a given number of rows and columns 
         *
         *	\param nRows: number of rows of the image to compute
         *	\param nCols: number of columns of the image to compute
         */
        virtual void resize(int nRows, int nCols) = 0;

        /** Initializes a pattern detector from a JSON file */
        void loadFromJSON(std::string filename);

        /** Estimates the pose of the pattern in an image stored in a double array          
         */
        virtual void compute(Eigen::ArrayXXd& image) = 0;

        /** Estimates the pose of the pattern in the image stored in a char array 
         *
         * @param data: pointer to the data of the pattern
         * @param rows: number of rows of the given data array
         * @param cols: number of cols of the given data array
         */
        void compute(char* data, int rows, int cols);

#ifdef USE_OPENCV
        /** Estimates the pose of the pattern in the image stored in a OpenCV Mat */
        void compute(cv::Mat& image);
#endif // USE_OPENCV

        /** Display the images to check the pattern detector working 
         * (do nothing if OpenCV is not available) */
        virtual void showControlImages() = 0;

        /** Returns the 2D pose of the pattern */
        virtual Pose get2DPose() = 0;

        /** Returns the most likely 3D pose of the pattern */
        virtual Pose get3DPose() = 0;

        /** Returns the four possible 3D pose the pattern */
        virtual std::vector<Pose> getAll3DPoses() = 0;

        /** Returns the attribute address corresponding to the given name */
        virtual void* getObject(const std::string & attribute);

        /** Returns the attribute value corresponding to the given name */
        virtual double getDouble(const std::string & attribute);

        /** Returns the attribute value corresponding to the given name */
        virtual int getInt(const std::string & attribute);

        /** Returns the attribute value corresponding to the given name */
        virtual std::string getString(const std::string & attribute);

        /** Sets the attribute value corresponding to the given name */
        virtual void setDouble(const std::string & attribute, double value);

        /** Sets the attribute value corresponding to the given name */
        virtual void setInt(const std::string & attribute, int value);

        /** Sets the attribute value corresponding to the given name */
        virtual void setString(const std::string & attribute, std::string value);
        
        /** Tells the detector to estimate the pose with a perspective projection */
        void setPerspectiveMode(bool isPerspective = true);
        
        /** Tells the detector to estimate the pose with an orthographic projection */
        void setOrthographicMode(bool isOrthographic = true);

    };

}

#endif