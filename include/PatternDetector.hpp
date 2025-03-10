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

        virtual void readJSON(rapidjson::Value& document);

        friend class Detector;

    public:

        /** Default constructor */
        PatternDetector();

        //        virtual ~PatternDetector() = default;

        //        /** Resizes the detector with a given number of rows and columns 
        //         *
        //         *	\param nRows: number of rows of the image to compute
        //         *	\param nCols: number of columns of the image to compute
        //         */
        //        virtual void resize(int nRows, int nCols) = 0;

        /** Initializes a pattern detector from a JSON file */
        void loadFromJSON(std::string filename);

        /** Estimates the pose of the pattern in an image stored in a double array          
         */
        virtual void compute(const Eigen::ArrayXXd & array);

        /** Estimates the pose of the pattern in the image stored in a char array 
         *
         * @param data: pointer to the data of the pattern
         * @param rows: number of rows of the given data array
         * @param cols: number of cols of the given data array
         */
        void compute(char* data, int rows, int cols);

        /** Estimates the pose of the pattern in the image stored in a OpenCV Mat */
        virtual void compute(const cv::Mat & image);
        virtual void compute2(const cv::Mat & image);

        /** Displays the images to check the pattern detector working. 
         * The function waits for a key event infinitely when delay == 0, or for 
         * delay milliseconds when delay > 0, or does not wait when delay < 0. */
        virtual void showControlImages(int delay = -1) = 0;

        /** Returns the 2D pose of the pattern */
        virtual Pose get2DPose(int id = 0) = 0;

        /** Returns the most likely 3D pose of the pattern */
        virtual Pose get3DPose(int id = 0) = 0;

        /** Returns true if the pattern has been detected and localized. */
        virtual bool patternFound(int id = 0) = 0;

        /** Returns the four possible 3D pose the pattern */
        virtual std::vector<Pose> getAll3DPoses(int id = 0) = 0;

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

    };

}

#endif