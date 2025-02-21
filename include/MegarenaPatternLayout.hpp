/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef MEGARENAPATTERNLAYOUT_HPP
#define MEGARENAPATTERNLAYOUT_HPP

#include "PeriodicPatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for Megarena patterns 
     *  
     */
    class MegarenaPatternLayout : public PeriodicPatternLayout {
    private:

        Eigen::ArrayXXi bitSequence;
        int codeDepth;

        void writeJSON(std::ofstream & file);

        void readJSON(rapidjson::Value & document);

    public:

        Rectangle regionOfInterest;
        
        MegarenaPatternLayout();

        MegarenaPatternLayout(double period, Eigen::ArrayXXi & bitSequence);

        MegarenaPatternLayout(double period, int codeDepth);

        void resize(double period);

        void toRectangleVector(std::vector<Rectangle> & rectangleList);

        double getIntensity(double x, double y);

        void saveToPNG(std::string filename = "");
        
        std::string toString();

        int getCodeDepth();
        
        int getInt(const std::string & attribute);
        
        void* getObject(const std::string & attribute);

    };

}

#endif