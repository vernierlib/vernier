/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef HPCODELAYOUT_HPP
#define HPCODELAYOUT_HPP

#include "PeriodicPatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for HPCode markers 
     */
    class HPCodeLayout : public PeriodicPatternLayout {
    protected:

        void addMarker(int row, int col, std::vector<Rectangle>& rectangleList);

        void writeJSON(std::ofstream & file);

        void readJSON(rapidjson::Value & document);

    public:

        HPCodeLayout();

        HPCodeLayout(double period, int size);

        void resize(double period, int size);

        double getIntensity(double x, double y);

        void toRectangleVector(std::vector<Rectangle>& rectangleList);

    };

}

#endif