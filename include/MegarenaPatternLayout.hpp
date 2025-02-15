/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
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

        Rectangle regionOfInterest;
        Eigen::ArrayXXi bitSequence;

        void writeJSON(std::ofstream & file);

        void readJSON(rapidjson::Value & document);

    public:

        MegarenaPatternLayout();

        MegarenaPatternLayout(double period, Eigen::ArrayXXi & bitSequence);

        MegarenaPatternLayout(double period, int codeSize);

        void resize(double period, Eigen::ArrayXXi & bitSequence);

        void toRectangleVector(std::vector<Rectangle> & rectangleList);

        double getIntensity(double x, double y);

        void saveToPNG(std::string filename = "");

        Rectangle getRegionOfInterest();

        void setRegionOfInterest(Rectangle regionOfInterest);
        
        void* getObject(const std::string & attribute);


    };

}

#endif