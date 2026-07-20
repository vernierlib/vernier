/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

#ifndef STAMPPATTERNLAYOUT_HPP
#define STAMPPATTERNLAYOUT_HPP

#include "BitmapPatternLayout.hpp"

namespace vernier {

    /** \brief Layout class for stamp patterns
     *  
     */
    class StampPatternLayout : public BitmapPatternLayout {
    protected:
        
        int id;
        
        void writeJSON(std::ofstream & file) override;

        void readJSON(const rapidjson::Value & document) override;        

    public:

        StampPatternLayout();

        StampPatternLayout(double period, int nRowsCols, int id);


    };

}

#endif