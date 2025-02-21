/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef FINGERPRINTPATTERNLAYOUT_HPP
#define FINGERPRINTPATTERNLAYOUT_HPP

#include "BitmapPatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for fingerprint patterns 
     *  
     */
    class FingerprintPatternLayout : public BitmapPatternLayout {
    protected:

        void readJSON(rapidjson::Value & document);

    public:

        FingerprintPatternLayout();

        FingerprintPatternLayout(double period, int nRows, int nCols);
        
        /** Construct a pattern from a PNG file with given period */
        FingerprintPatternLayout(std::string filename, double period);

        void resize(double dotSize, int nRows, int nCols);

        double getIntensity(double x, double y);

        void toRectangleVector(std::vector<Rectangle>& rectangleList);

    };

}

#endif