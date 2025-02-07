/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "PatternLayout.hpp"
#include "PeriodicPatternLayout.hpp"
#include "BitmapPatternLayout.hpp"
#include "StampMarkerLayout.hpp"
#include "MegarenaPatternLayout.hpp"
#include "HPCodeLayout.hpp"
#include "CustomPatternLayout.hpp"

namespace vernier {

    class Layout {
    public:

        static PatternLayout * newInstance(const std::string& classname) {
            PatternLayout* layout;
            if (classname == "PeriodicPattern") {
                layout = new PeriodicPatternLayout();
            } else if (classname == "BitmapPattern") {
                layout = new BitmapPatternLayout();
            } else if (classname == "FingerprintPattern") {
                layout = new StampMarkerLayout();
            } else if (classname == "MegarenaPattern") {
                layout = new MegarenaPatternLayout();
            } else if (classname == "QRCodePattern") {
                layout = new HPCodeLayout();
            } else if (classname == "CustomPattern") {
                layout = new CustomPatternLayout();
            } else {
                throw Exception(classname + " is not a valid class name for a pattern layout.");
            }
            return layout;
        }

        static PatternLayout * loadFromJSON(const std::string& filename) {
            BufferedReader bufferedReader(filename);
            rapidjson::Document document;
            document.ParseInsitu(bufferedReader.data());
            if (!document.IsObject()) {
                throw Exception(filename + " is not a valid JSON file.");
            }
            if (document.MemberBegin() == document.MemberEnd()) {
                throw Exception(filename + " is empty.");
            }
            std::string classname = document.MemberBegin()->name.GetString();
            PatternLayout* layout = newInstance(classname);
            layout->readJSON(document.MemberBegin()->value);
            return layout;
        }

    };

}

#endif