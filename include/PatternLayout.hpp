/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef PATTERNLAYOUT_HPP
#define PATTERNLAYOUT_HPP

#include "Common.hpp"
#ifndef WIN32
#include <gdstk/gdstk.hpp>
#endif

namespace vernier {

    /** \brief Abstract class to define the interface of pattern detectors. 
     * 
     * \example generatingPatternLayout.cpp 
     * \example renderingPatternImage.cpp
     * 
     */
    class PatternLayout {
    protected:

        std::string classname;
        double width;
        double height;
        double originX;
        double originY;
        
        virtual void writeJSON(std::ofstream & file);

        void writeApproxPxPeriod(std::ofstream& file);

        virtual void readJSON(const rapidjson::Value & document);

        friend class Layout;

    public:

        std::string description;
        std::string date;
        std::string author;
        std::string unit;
        double leftMargin;
        double rightMargin;
        double topMargin;
        double bottomMargin;
        
        PatternLayout();

        virtual ~PatternLayout() {
        };

        /** Initializes a pattern layout from a JSON file */
        void loadFromJSON(const std::string & filename);

        /** Creates a SVG file corresponding to the pattern layout */
        void saveToSVG(std::string filename = "");

        /** Creates a macro file for LayoutEditor corresponding to the pattern layout */
        void saveToLayoutEditorMacro(std::string filename = "");

#ifndef WIN32
        /** Creates a SVG file corresponding to the pattern layout */
        void saveToGDS(std::string filename = "");

        /** Creates a OASIS file corresponding to the pattern layout */
        void saveToOASIS(std::string filename = "");
        
        /** Creates a new GDS cell corresponding to the pattern layout (must be deleted afterward) */
        gdstk::Cell * convertToGDSCell(std::string name = "");
#endif

        /** Creates a CSV file listing all the dots of the pattern layout */
        void saveToCSV(std::string filename = "");

        /** Creates a JSON file corresponding to the pattern layout */
        void saveToJSON(std::string filename = "");

        /** Creates a PNG file corresponding to the pattern layout (requires OpenCV) */
        virtual void saveToPNG(std::string filename = "");

        /** Returns the intensity (between 0.0 and 1.0) of the pattern at point (x,y) */
        virtual double getIntensity(double x, double y) = 0;

        /** Returns a vector listing all the dots of the pattern */
        virtual void toRectangleVector(std::vector<Rectangle>& rectangleList) = 0;

        /** Renders an image with an orthographic projection defined by:
         * 
         *       [ u ]   [ 1/s  0    0  cx ]         [ x ]
         *       [ v ] = [ 0    1/s  0  cy ] * cTp * [ y ]
         *       [ 1 ]   [ 0    0    0   1 ]         [ z ]
         *                                           [ 1 ]
         * 
         *   with cx and cy the coordinates of the principal point (in pixels) and s the scale factor (pixel size)
         * 
         *	\param pose: pattern pose (defines cTp and the scale factor)
         *	\param outputImage: any size double array 
         *      \param principalPoint (optional): coordinates of the optical center on the sensor, by default they are calculated to be the image center coordinates.
         *
         */
        void renderOrthographicProjection(Pose pose, Eigen::ArrayXXd & outputImage, Eigen::Vector2d principalPoint = Eigen::Vector2d(-1.0, -1.0));

        void renderOrthographicProjection(Pose pose, cv::Mat & outputImage, Eigen::Vector2d principalPoint = Eigen::Vector2d(-1.0, -1.0));
        
        /** Renders an image with a perspective projection defined by the pinhole camera model:
         * 
         *        [ u ]   [ f/s  0  cx  0 ]         [ x ]
         *        [ v ] = [ 0  f/s  cy  0 ] * cTp * [ y ]
         *        [ 1 ]   [ 0    0   1  0 ]         [ z ]
         *                                          [ 1 ]
         * 
         *   with cx and cy the coordinates of the principal point (in pixels), f the focal length (metric) and s the scale factor (pixel size)
         * 
         *	\param pose: pattern pose (defines cTp and the scale factor)
         *	\param outputImage: any size double array 
         *      \param focalLength: focal length of the camera
         *      \param principalPoint (optional): coordinates of the optical center on the sensor, by default they are calculated to be the image center coordinates.
         * 
         */
        void renderPerspectiveProjection(Pose pose, Eigen::ArrayXXd & outputImage, double focalLength, Eigen::Vector2d principalPoint = Eigen::Vector2d(-1.0, -1.0));
        
        void renderPerspectiveProjection(Pose pose, cv::Mat & outputImage, double focalLength, Eigen::Vector2d principalPoint = Eigen::Vector2d(-1.0, -1.0));

        virtual std::string toString();
        
        std::string getClassname();

        double getOriginX();

        double getOriginY();

        double getHeight();

        double getWidth();
        
        void setMargins(double margins);

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