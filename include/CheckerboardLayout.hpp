/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

#ifndef CHECKERBOARDLAYOUT_HPP
#define CHECKERBOARDLAYOUT_HPP

#include "PatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for checkerboard calibration pattern
     *  
     */
    class CheckerboardLayout : public PatternLayout {
    protected:
        
        void writeJSON(std::ofstream & file) override;

        void readJSON(const rapidjson::Value & document) override;
        
        int nRows, nCols;
        double squareSize;

    public:

        CheckerboardLayout();
        
        CheckerboardLayout(double squareSize, int nRows, int nCols);
        
        void resize(double squareSize, int nRows, int nCols);

        double getIntensity(double x, double y) override;

        double getPhase1(double x, double y) override;

        double getPhase2(double x, double y) override;

        void toRectangleVector(std::vector<Rectangle>& rectangleList) override;

    };

}

#endif