/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "StampPatternDetector.hpp"
#include "Spatial.hpp"

namespace vernier {

    StampPatternDetector::StampPatternDetector()
    : BitmapPatternDetector() {
        classname = "StampPattern";
    }

    StampPatternDetector::StampPatternDetector(double physicalPeriod, const std::string & filename, int snapshotSize)
    : BitmapPatternDetector(physicalPeriod, filename) {
        classname = "StampPattern";
        ASSERT_MSG(bitmap[0].cols == bitmap[0].rows, "The stamp bitmap must be square");
        snapshot.resize(snapshotSize, snapshotSize);
        window = hannWindow(snapshotSize, 4);
        bitmapThumbnail.resize(bitmap[0].cols - 8);
        patternPhase.resize(snapshotSize, snapshotSize);
    }

    void StampPatternDetector::readJSON(const rapidjson::Value& document) {
        throw Exception("StampPatternDetector::readJSON is not implemented yet.");
    }

    void StampPatternDetector::computeImage() {

        detector.compute(image8U);

        markers.clear();
        for (int i = 0; i < detector.squares.size(); i++) {

            Square square = detector.squares[i];
            int diameter = (int) (2 * square.getRadius());
            if (snapshot.cols() < diameter) {
                std::cout << "The stamp is too large for pose estimation: increase the snapshot size over " << diameter << " pixels." << std::endl;
            }
            if (2 * diameter < bitmapThumbnail.size()) {
                std::cout << "The stamp is too tiny for pose estimation: increase the picture quality size." << std::endl;
            }

            int centerX = (int) square.getCenter().x;
            int centerY = (int) square.getCenter().y;
            takeSnapshot(centerX, centerY, snapshot.cols(), array, snapshot);

            patternPhase.compute(snapshot * window);
            plane1 = patternPhase.getPlane1();
            plane2 = patternPhase.getPlane2();

            if (patternPhase.peaksFound()) {

                bitmapThumbnail.compute(snapshot.real(), patternPhase.getPlane1(), patternPhase.getPlane2());
                computeAbsolutePose();

                double dx = -plane1.getPosition(physicalPeriod, 0.0, 0.0, periodShift1);
                double dy = -plane2.getPosition(physicalPeriod, 0.0, 0.0, periodShift2);
                double alpha = plane1.getAngle();

                double pixelSize = physicalPeriod / patternPhase.getPixelPeriod();
                double xImg = (centerX - image8U.cols / 2);
                double yImg = (centerY - image8U.rows / 2);
                double x = pixelSize * (xImg * cos(alpha) - yImg * sin(-alpha)) + dx;
                double y = pixelSize * (xImg * sin(-alpha) + yImg * cos(alpha)) + dy;

                Pose pose = Pose(x, y, alpha, pixelSize);

                int id = bitmapThumbnail.hashCode(maxAngle);
                markers.insert(std::make_pair(id, pose));
            }

        }
    }

    Pose StampPatternDetector::get2DPose(int id) {
        if (id < 0) {
            return markers.begin()->second;
        } else {
            return markers.at(id);
        }
    }

    bool StampPatternDetector::patternFound(int id) {
        if (id < 0) {
            return (markers.size() > 0);
        } else {
            return (markers.find(id) != markers.end());
        }
    }

    int StampPatternDetector::patternCount() {
        return markers.size();
    };

    Pose StampPatternDetector::get3DPose(int id) {
        throw Exception("StampPatternDetector::get3DPose is not implemented yet.");
        return Pose();
    }

    std::vector<Pose> StampPatternDetector::getAll3DPoses(int id) {
        throw Exception("StampPatternDetector::getAll3DPoses is not implemented yet.");
        return std::vector<Pose> ();
    }

    void StampPatternDetector::showControlImages() {
        patternPhase.showControlImages();
        bitmapThumbnail.showControlImages();
    }

    void StampPatternDetector::draw(cv::Mat & image) {
        detector.draw(image);
        for (std::map<int, Pose>::iterator it = markers.begin(); it != markers.end(); it++) {
            it->second.draw(image, snapshot.cols() / 2, to_string(it->first));
        }
    }

}