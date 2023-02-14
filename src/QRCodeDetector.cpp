/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "QRCodeDetector.hpp"

namespace Vernier {
#ifdef USE_OPENCV

    void QRCodeDetector::computeDistances() {
        int size = markerDetector.markers.size();
        distance.resize(size);
        for (int i = 0; i < size; i++) {
            distance[i].resize(size);
        }
        for (int i = 0; i < size; i++) {
            distance[i][i] = 0.0;
            for (int j = i + 1; j < size; j++) {
                distance[i][j] = distance[j][i] = cv::norm(markerDetector.markers[i].position - markerDetector.markers[j].position);
            }
        }
    }

    void QRCodeDetector::makeFirstClustering() {
        int clusterCount = markerDetector.markers.size() / 3;
        clusters.resize(clusterCount);

        int markerCount = clusterCount * 3;
        std::vector<bool> assigned(markerCount);
        for (int i = 0; i < assigned.size(); i++) {
            assigned[i] = false;
        }

        int currentCluster = 0;
        while (currentCluster < clusterCount) {
            clusters[currentCluster].clear();
            // looking for a not assigned marker
            int a = 0;
            while (a < markerCount && assigned[a]) a++;

            clusters[currentCluster].push_back(a);
            assigned[a] = true;

            // looking for a first closest marker
            int b;
            double min = 1e150;
            for (int i = 0; i < markerCount; i++) {
                if (!assigned[i] && distance[a][i] < min) {
                    b = i;
                    min = distance[a][i];
                }
            }
            clusters[currentCluster].push_back(b);
            assigned[b] = true;

            // looking for a second closest marker
            int c;
            min = 1e150;
            for (int i = 0; i < markerCount; i++) {
                if (!assigned[i] && distance[a][i] < min) {
                    c = i;
                    min = distance[a][i];
                }
            }
            clusters[currentCluster].push_back(c);
            assigned[c] = true;

            currentCluster++;
        }
    }

    void QRCodeDetector::recordCodes() {
        codes.clear();
        for (int i = 0; i < clusters.size(); i++) {
            QRCode code(markerDetector.markers[clusters[i][0]], markerDetector.markers[clusters[i][1]], markerDetector.markers[clusters[i][2]]);

            //double rotationDistance;
            //cv::Point2d refright(code.right.x - code.top.x, code.right.y - code.top.y);
            //cv::Point2d refBottom(code.bottom.x - code.top.x, code.bottom.y - code.top.y);
            //rotationDistance = cv::norm(cv::Point2d(refBottom.y, -refBottom.x) - cv::Point2d(refright.x, refright.y));

            codes.push_back(code);
        }
    }

    void QRCodeDetector::compute(cv::Mat& image) {
        markerDetector.compute(image);
        codes.clear();
        if (markerDetector.markers.size() >= 3) {
            computeDistances();
            makeFirstClustering();
            //refineClusteringUsingKMeans();
            recordCodes();
        }
    }

    void QRCodeDetector::draw(cv::Mat& image) {
        for (int i = 0; i < codes.size(); i++) {
            codes[i].draw(image);
        }
    }

    std::string QRCodeDetector::toString() {
        std::string result = "{ ";
        for (int i = 0; i < codes.size(); i++) {
            result += codes[i].toString() + " ; ";
        }
        result += " }";
        return result;
    }
#endif //USE_OPENCV
}