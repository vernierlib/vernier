/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef MEGARENAPATTERNLAYOUT_HPP
#define MEGARENAPATTERNLAYOUT_HPP

#include "PeriodicPatternLayout.hpp"

namespace Vernier {

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

#ifdef USE_OPENCV
        void saveToPNG(std::string filename = "");
#endif // USE_OPENCV

        Rectangle getRegionOfInterest() const {
            return regionOfInterest;
        }

        void setRegionOfInterest(Rectangle regionOfInterest) {
            this->regionOfInterest = regionOfInterest;
        }

    };

}

#endif