/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#ifndef STAMPPATTERNDETECTOR_HPP
#define STAMPPATTERNDETECTOR_HPP

#include "BitmapPatternDetector.hpp"
#include "SquareDetector.hpp"
#include "PatternPhase.hpp"
#include "BitmapThumbnail.hpp"
#include <map>

namespace vernier {

    class StampPatternDetector : public BitmapPatternDetector {
    protected:
        
        Eigen::ArrayXXd window;
        Eigen::ArrayXXd snapshot;

        void readJSON(const rapidjson::Value& document) override;

        void computeImage() override;

    public:

        SquareDetector detector;
        
        /** Map of detected stamps (key map = stamp id) */
        std::map<int, Pose> markers;
        
        StampPatternDetector();

        /** Constructs a pose estimator for stamps
         *
         *	\param physicalPeriod: physical period between the dots of the stamp
         *	\param filename: name of the bitmap file
         *	\param snapshotSize: maximal size of diagonal of the stamp in pixels (used to cut the image)
         */
        StampPatternDetector(double physicalPeriod, const std::string & filename, int snapshotSize);

        void addBitmap(const std::string & filename);

        Pose get2DPose(int id = -1) override;

        Pose get3DPose(int id = -1) override;

        std::vector<Pose> getAll3DPoses(int id = -1) override;

        bool patternFound(int id = -1) override;
        
        int patternCount() override;

        void draw(cv::Mat& image) override;

        void showControlImages() override;

    };

}

#endif