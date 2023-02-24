/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef PERIODICPATTERNLAYOUT_HPP
#define PERIODICPATTERNLAYOUT_HPP

#include "PatternLayout.hpp"

namespace Vernier {

    /** \brief Layout class for periodic patterns 
     */
    class PeriodicPatternLayout : public PatternLayout {
    protected:

        double period;
        double dotSize;
        int nRows;
        int nCols;

        void writeJSON(std::ofstream & file);

        void readJSON(rapidjson::Value & document);

    public:

        PeriodicPatternLayout();

        PeriodicPatternLayout(double period, int nRows, int nCols);

        void resize(double period, int nRows, int nCols);

        double getIntensity(double x, double y);

        void toRectangleVector(std::vector<Rectangle>& rectangleList);

        void saveToPNG(std::string filename = "");
        
        double getDouble(const std::string & attribute);

        int getInt(const std::string & attribute);


    };

}

#endif