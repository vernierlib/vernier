/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef HPCODEPATTERNLAYOUT_HPP
#define HPCODEPATTERNLAYOUT_HPP

#include "PeriodicPatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for HPCode markers 
     */
    class HPCodePatternLayout : public PeriodicPatternLayout {
    protected:

        void addMarker(int row, int col, std::vector<Rectangle>& rectangleList);

        void writeJSON(std::ofstream & file);

        void readJSON(rapidjson::Value & document);

    public:

        HPCodePatternLayout();

        HPCodePatternLayout(double period, int size);

        void resize(double period, int size);

        double getIntensity(double x, double y);

        void toRectangleVector(std::vector<Rectangle>& rectangleList);
        
        std::string toString();

    };

}

#endif