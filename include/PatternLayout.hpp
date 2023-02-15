/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef PATTERNLAYOUT_HPP
#define PATTERNLAYOUT_HPP

#include "Common.hpp"

namespace Vernier {

    /** \brief Abstract class to define the interface of pattern detectors. 
     *  
     * \example renderingPatternIn2D.cpp
     * \example renderingPatternIn3D.cpp
     * \example generatingPNGfromJSON.cpp
     * \example generatingSVGfromJSON.cpp
     */
    class PatternLayout {
    protected:

        std::string classname;
        std::string description;
        std::string date;
        std::string author;
        std::string unit;
        double originX;
        double originY;
        double margin;
        double width;
        double height;

        virtual void writeJSON(std::ofstream & file);

        void writeApproxPxPeriod(std::ofstream& file);

        virtual void readJSON(rapidjson::Value & document);

        friend class Layout;

    public:

        PatternLayout();

        virtual ~PatternLayout() {
        };

        /** Creates a SVG file corresponding to the pattern layout */
        void saveToSVG(std::string filename = "");

        /** Creates a macro for LayoutEditor corresponding to the pattern layout */
        void saveToLayoutEditorMacro(std::string filename = "");

        /** Creates a CSV file listing all the dots of the pattern layout */
        void saveToCSV(std::string filename = "");

        /** Creates a JSON file corresponding to the pattern layout */
        void saveToJSON(std::string filename = "");

        /** Creates a PNG file corresponding to the pattern layout (requires OpenCV) */
        virtual void saveToPNG(std::string filename = "");

        /** Initializes a pattern layout from a JSON file */
        void loadFromJSON(std::string filename);

        /** Returns the intensity (between 0.0 and 1.0) of the pattern at point (x,y) */
        virtual double getIntensity(double x, double y) = 0;

        /** Returns a vector listing all the dots of the pattern */
        virtual void toRectangleVector(std::vector<Rectangle>& rectangleList) = 0;

        /** Renders an image with an orthographic projection defined by:
         * 
         *       [ u ]   [ 1  0  0  cx ]         [ x ]
         *   s * [ v ] = [ 0  1  0  cy ] * cTp * [ y ]
         *       [ 1 ]   [ 0  0  0   1 ]         [ z ]
         *                                       [ 1 ]
         * 
         *   with cx and cy the coordinates of the image center and s the scale factor (pixel size)
         * 
         *	\param pose: pattern pose (defines cTp)
         *	\param outputImage: any size double array 
         */
        void renderOrthographicProjection(Pose pose, Eigen::ArrayXXd & outputImage);

        /** Renders an image with a perspective projection defined by the pinhole camera model:
         * 
         *        [ u ]   [ f  0  cx  0 ]         [ x ]
         *    s * [ v ] = [ 0  f  cy  0 ] * cTp * [ y ]
         *        [ 1 ]   [ 0  0   1  0 ]         [ z ]
         *                                        [ 1 ]
         * 
         *   with cx and cy the coordinates of the image center, f the focal length and s the scale factor (pixel size)
         * 
         *	\param pose: pattern pose (defines cTp)
         *	\param outputImage: any size double array 
         *  
         *   cx and cy are calculated to be the image center coordinates.
         */
        void renderPerspectiveProjection(Pose pose, double focalLength, Eigen::ArrayXXd & outputImage);

        std::string getAuthor() const {
            return author;
        }

        std::string getDate() const {
            return date;
        }

        std::string getDescription() const {
            return description;
        }

        std::string getUnit() const {
            return unit;
        }

        std::string getClassname() const {
            return classname;
        }

        double getOriginX() const {
            return originX;
        }

        double getOriginY() const {
            return originY;
        }

        double getMargin() const {
            return margin;
        }

        double getHeight() const {
            return height;
        }

        double getWidth() const {
            return width;
        }

        void setAuthor(std::string author) {
            this->author = author;
        }

        void setDate(std::string date) {
            this->date = date;
        }

        void setDescription(std::string description) {
            this->description = description;
        }

        void setUnit(std::string unit) {
            this->unit = unit;
        }

        void setMargin(double margin) {
            this->margin = margin;
        }

    };

}

#endif