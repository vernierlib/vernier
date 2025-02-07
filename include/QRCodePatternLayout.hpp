/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef QRCODEPATTERNLAYOUT_HPP
#define QRCODEPATTERNLAYOUT_HPP

#include "PeriodicPatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for QRCode patterns 
     */
    class QRCodePatternLayout : public PeriodicPatternLayout {
    protected:

        void addMarker(int row, int col, std::vector<Rectangle>& rectangleList);

        void writeJSON(std::ofstream & file);

        void readJSON(rapidjson::Value & document);

    public:

        QRCodePatternLayout();

        QRCodePatternLayout(double period, int size);

        void resize(double period, int size);

        double getIntensity(double x, double y);

        void toRectangleVector(std::vector<Rectangle>& rectangleList);

    };

}

#endif