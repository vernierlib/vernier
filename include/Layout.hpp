/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "PatternLayout.hpp"
#include "PeriodicPatternLayout.hpp"
#include "BitmapPatternLayout.hpp"
#include "StampPatternLayout.hpp"
#include "MegarenaPatternLayout.hpp"
#include "HPCodePatternLayout.hpp"
#include "CustomPatternLayout.hpp"
#include "CheckerboardLayout.hpp"
#include "CheckerTagLayout.hpp"

namespace vernier {

    class Layout {
    public:

        static PatternLayout * newInstance(const std::string& classname) {
            PatternLayout* layout;
            if (classname == "PeriodicPattern") {
                layout = new PeriodicPatternLayout();
            } else if (classname == "BitmapPattern") {
                layout = new BitmapPatternLayout();
            } else if (classname == "StampPattern") {
                layout = new StampPatternLayout();
            } else if (classname == "MegarenaPattern") {
                layout = new MegarenaPatternLayout();
            } else if (classname == "HPCodePattern") {
                layout = new HPCodePatternLayout();
            } else if (classname == "CustomPattern") {
                layout = new CustomPatternLayout();
            } else if (classname == "Checkerboard") {
                layout = new CheckerboardLayout();
            } else if (classname == "CheckerTag") {
                layout = new CheckerTagLayout();
            } else {
                throw Exception(classname + " is not a valid class name for a pattern layout.");
            }
            return layout;
        }

        static PatternLayout * loadFromJSON(const std::string& filename) {
            BufferedReader bufferedReader(filename);
            rapidjson::Document document;
            bufferedReader.parseJSON(document);
            std::string classname = document.MemberBegin()->name.GetString();
            PatternLayout* layout = newInstance(classname);
            layout->readJSON(document.MemberBegin()->value);
            return layout;
        }
        
    };

}

#endif