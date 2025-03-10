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

    public:

        double dotSize;
        
        PeriodicPatternLayout();

        PeriodicPatternLayout(double period, int nRows, int nCols);

        void resize(double period, int nRows, int nCols);

        double getIntensity(double x, double y) override;

        void toRectangleVector(std::vector<Rectangle>& rectangleList) override;

        void saveToPNG(const std::string filename = "") override;
        
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