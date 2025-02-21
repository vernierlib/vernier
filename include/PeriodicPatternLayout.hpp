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

        void writeJSON(std::ofstream & file);

        void readJSON(rapidjson::Value & document);

    public:

        double dotSize;
        
        PeriodicPatternLayout();

        PeriodicPatternLayout(double period, int nRows, int nCols);

        void resize(double period, int nRows, int nCols);

        double getIntensity(double x, double y);

        void toRectangleVector(std::vector<Rectangle>& rectangleList);

        void saveToPNG(std::string filename = "");
        
        std::string toString();
        
        double getPeriod();
        
        int getNRows();
        
        int getNCols();
        
        double getDouble(const std::string & attribute);

        int getInt(const std::string & attribute);
        
        void setDouble(const std::string & attribute, double value);

    };

}

#endif