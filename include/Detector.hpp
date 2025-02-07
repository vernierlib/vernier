/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "PatternDetector.hpp"
#include "PeriodicPatternDetector.hpp"
#include "MegarenaPatternDetector.hpp"
#include "BitmapPatternDetector.hpp"
#include "QRCodePatternDetector.hpp"

namespace vernier {

    /** \brief Class factory to construct any pattern detectors.
     * 
     *   This class uses an external JSON file to create the corresponding instance of detector.
     */
    class Detector {
    public:

        /** Create an instance of pattern detector given its classname.
         *   
         *   \param classname: name of the pattern detector (PeriodicPattern, MegarenaPattern, BitmapPattern, QRCodePattern).
         **/
        static PatternDetector* newInstance(const std::string& classname) {
            PatternDetector* detector;
            if (classname == "PeriodicPattern") {
                detector = new PeriodicPatternDetector();
            } else if (classname == "MegarenaPattern") {
                detector = new MegarenaPatternDetector();
            } else if (classname == "BitmapPattern") {
                detector = new BitmapPatternDetector();
            } else if (classname == "QRCodePattern") {
                detector = new QRCodePatternDetector();
            } else {
                throw Exception(classname + " is not a valid class name for a pattern detector.");
            }
            return detector;
        }

        /** Load all the relevant information that are contained in the JSON document and creates the respective instance of pattern detector.
         *
         *   \param filename: name of the JSON document to load
         **/
        static PatternDetector * loadFromJSON(const std::string& filename) {
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
            PatternDetector* detector = newInstance(classname);
            detector->readJSON(document.MemberBegin()->value);
            return detector;
        }

    };
}
#endif // !DETECTOR_HPP