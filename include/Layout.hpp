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
#include <memory>

namespace vernier {

    class Layout {
    public:

        static std::unique_ptr<PatternLayout> newInstance(const std::string& classname) {
            std::unique_ptr<PatternLayout> layout;
            if (classname == "PeriodicPattern") {
                layout.reset(new PeriodicPatternLayout());
            } else if (classname == "BitmapPattern") {
                layout.reset(new BitmapPatternLayout());
            } else if (classname == "StampPattern") {
                layout.reset(new StampPatternLayout());
            } else if (classname == "MegarenaPattern") {
                layout.reset(new MegarenaPatternLayout());
            } else if (classname == "HPCodePattern") {
                layout.reset(new HPCodePatternLayout());
            } else if (classname == "CustomPattern") {
                layout.reset(new CustomPatternLayout());
            } else if (classname == "Checkerboard") {
                layout.reset(new CheckerboardLayout());
            } else if (classname == "CheckerTag") {
                layout.reset(new CheckerTagLayout());
            } else {
                throw Exception(classname + " is not a valid class name for a pattern layout.");
            }
            return layout;
        }

        static std::unique_ptr<PatternLayout> loadFromJSON(const std::string& filename) {
            BufferedReader bufferedReader(filename);
            rapidjson::Document document;
            bufferedReader.parseJSON(document);
            std::string classname = document.MemberBegin()->name.GetString();
            std::unique_ptr<PatternLayout> layout = newInstance(classname);
            layout->readJSON(document.MemberBegin()->value);
            return layout;
        }
        
    };

}

#endif