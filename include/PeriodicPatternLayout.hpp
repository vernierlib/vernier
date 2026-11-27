/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef PERIODICPATTERNLAYOUT_HPP
#define PERIODICPATTERNLAYOUT_HPP

#include "PatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for periodic patterns 
     */
    class PeriodicPatternLayout : public PatternLayout {
    protected:

        double period;
        int nRows;
        int nCols;

        void writeJSON(std::ofstream & file) override;

        void readJSON(const rapidjson::Value & document) override;

        cv::Mat getPNGCells() override;

        /** Returns 1 if the cell (row, col) is a dot and 0 otherwise.
         *
         *  The pattern is a grid of 2*nRows-1 by 2*nCols-1 square cells of dotSize side,
         *  any cell outside the grid is blank. */
        virtual int getCell(int row, int col);

        /** Returns the intensity at point (x,y) by cosine interpolation between the four
         *  cells around the point, so that adjacent dots merge into one smooth shape. */
        double interpolateCells(double x, double y);

    public:

        double dotSize;
        
        PeriodicPatternLayout();

        PeriodicPatternLayout(double period, int nRows, int nCols);

        void resize(double period, int nRows, int nCols);

        double getIntensity(double x, double y) override;

        double getPhase1(double x, double y) override;

        double getPhase2(double x, double y) override;

        void toRectangleVector(std::vector<Rectangle>& rectangleList) override;

        std::string toString() override;
        
        double getPeriod();
        
        int getNRows();
        
        int getNCols();
        
        double getDouble(const std::string & attribute) override;

        int getInt(const std::string & attribute) override;
        
        void setDouble(const std::string & attribute, double value) override;

    };

}

#endif