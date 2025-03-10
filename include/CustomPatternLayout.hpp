/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef CUSTOMPATTERNLAYOUT_HPP
#define CUSTOMPATTERNLAYOUT_HPP

#include "PatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for custom patterns (not periodic)
     *  
     */
    class CustomPatternLayout : public PatternLayout {
    protected:
        
        std::vector<Rectangle> dots;
        std::vector<double> dotsIntensity;

        void writeJSON(std::ofstream & file) override;

        void readJSON(const rapidjson::Value & document) override;

    public:

        CustomPatternLayout();

        void resize();

        /** Initializes a pattern from a CSV file */
        virtual void loadFromCSV(std::string filename);

        double getIntensity(double x, double y) override;

        void toRectangleVector(std::vector<Rectangle>& rectangleList) override;

    };

}

#endif