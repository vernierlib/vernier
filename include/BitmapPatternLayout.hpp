/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef BITMAPPATTERNLAYOUT_HPP
#define BITMAPPATTERNLAYOUT_HPP

#include "PeriodicPatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for bitmap patterns 
     *  
     */
    class BitmapPatternLayout : public PeriodicPatternLayout {
    protected:

        Eigen::ArrayXXi bitmap;

        void writeJSON(std::ofstream & file);

        void readJSON(rapidjson::Value & document);

    public:

        BitmapPatternLayout();

        BitmapPatternLayout(double period, int nRows, int nCols);

        /** Construct a pattern from a PNG file with given period */
        BitmapPatternLayout(std::string filename, double period);

        void resize(double period, int nRows, int nCols);

        double getIntensity(double x, double y);

        void toRectangleVector(std::vector<Rectangle>& rectangleList);
        
        void saveToPNG(std::string filename = "");

        inline void setDot(int row, int col, int value) {
            bitmap(row, col) = value;
        }

        inline int getDot(int row, int col) {
            return bitmap(row, col);
        }
        
        int numberOfWrongEdges();
        
        bool hasWrongEdges();
        
        int numberOfCorrectEdges();

    };

}

#endif