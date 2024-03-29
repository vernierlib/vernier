/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef FINGERPRINTPATTERNLAYOUT_HPP
#define FINGERPRINTPATTERNLAYOUT_HPP

#include "PeriodicPatternLayout.hpp"

namespace Vernier {

    /** \brief Layout class for finger print patterns 
     *  
     */
    class FingerprintPatternLayout : public PeriodicPatternLayout {
    protected:

        Eigen::ArrayXXi bitmap;

        void writeJSON(std::ofstream & file);

        void readJSON(rapidjson::Value & document);

    public:

        FingerprintPatternLayout();

        FingerprintPatternLayout(double period, int nRows, int nCols);

        void resize(double dotSize, int nRows, int nCols);

        /** Initializes a pattern from a PNG file with given period (requires OpenCV) */
        virtual void loadFromPNG(std::string filename, double period);

        double getIntensity(double x, double y);

        void toRectangleVector(std::vector<Rectangle>& rectangleList);

        void setDot(int row, int col, int value) {
            bitmap(row, col) = value;
        }

        int getDot(int row, int col) {
            return bitmap(row, col);
        }
    };

}

#endif