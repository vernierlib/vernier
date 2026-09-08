/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "PatternDetector.hpp"
#include "PeriodicPatternDetector.hpp"
#include "MegarenaPatternDetector.hpp"
#include "BitmapPatternDetector.hpp"
#include "HPCodePatternDetector.hpp"
#include "StampPatternDetector.hpp"
#include <memory>

namespace vernier {

    /** \brief Class factory to construct any pattern detectors.
     * 
     *   This class uses an external JSON file to create the corresponding instance of detector.
     */
    class Detector {
    public:

        /** Create an instance of pattern detector given its classname.
         *   
         *   \param classname: name of the pattern detector (PeriodicPattern, MegarenaPattern, BitmapPattern, HPCodePattern).
         **/
        static std::unique_ptr<PatternDetector> newInstance(const std::string& classname) {
            std::unique_ptr<PatternDetector> detector;
            if (classname == "PeriodicPattern") {
                detector.reset(new PeriodicPatternDetector());
            } else if (classname == "MegarenaPattern") {
                detector.reset(new MegarenaPatternDetector());
            } else if (classname == "BitmapPattern") {
                detector.reset(new BitmapPatternDetector());
            } else if (classname == "HPCodePattern") {
                detector.reset(new HPCodePatternDetector());
            } else if (classname == "StampPattern") {
                detector.reset(new StampPatternDetector());
            } else {
                throw Exception(classname + " is not a valid class name for a pattern detector.");
            }
            return detector;
        }

        /** Load all the relevant information that are contained in the JSON document and creates the respective instance of pattern detector.
         *
         *   \param filename: name of the JSON document to load
         **/
        static std::unique_ptr<PatternDetector> loadFromJSON(const std::string& filename) {
            BufferedReader bufferedReader(filename);
            rapidjson::Document document;
            bufferedReader.parseJSON(document);
            std::string classname = document.MemberBegin()->name.GetString();
            std::unique_ptr<PatternDetector> detector = newInstance(classname);
            detector->readJSON(document.MemberBegin()->value);
            return detector;
        }

    };
}
#endif