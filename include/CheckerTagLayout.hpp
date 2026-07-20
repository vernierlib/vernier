/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

#ifndef CHECKERTAGLAYOUT_HPP
#define CHECKERTAGLAYOUT_HPP

#include "StampPatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for checker tags
     *  
     */
    class CheckerTagLayout : public StampPatternLayout {
    protected:
        
        void writeJSON(std::ofstream & file) override;

        void readJSON(const rapidjson::Value & document) override;
        

    public:

        CheckerTagLayout();
        
        CheckerTagLayout(double dotSize, int nRowsCols, int id);

//        double getPhase1(double x, double y) override;
//
//        double getPhase2(double x, double y) override;

    };

}

#endif