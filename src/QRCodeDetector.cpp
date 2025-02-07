/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "QRCodeDetector.hpp"

namespace vernier {
#ifdef USE_OPENCV

    QRCode::QRCode(QRFiducialPattern& marker0, QRFiducialPattern& marker1, QRFiducialPattern& marker2) {
        // Determining the top marker
        double d01 = cv::norm(marker0.position - marker1.position);
        double d12 = cv::norm(marker1.position - marker2.position);
        double d02 = cv::norm(marker0.position - marker2.position);

        if (d01 > d02 && d01 > d12) {
            top = marker2.position;
            right = marker0.position;
            bottom = marker1.position;
        } else if (d12 > d01 && d12 > d02) {
            top = marker0.position;
            right = marker1.position;
            bottom = marker2.position;
        } else {
            top = marker1.position;
            right = marker0.position;
            bottom = marker2.position;
        }

        // Determining the positive sense of rotation
        cv::Point2d rightDirection = right - top;
        if (rightDirection.cross(bottom - top) < 0) {
            std::swap(right, bottom);
            rightDirection = right - top;
        }
        
        this->center = cv::Point2d((right.x + bottom.x) / 2.0, (right.y + bottom.y) / 2.0);
    }

    double QRCode::getAngle() {
        cv::Point2d rightDirection = right - top;
        return atan2(rightDirection.y, rightDirection.x);
    }

    void QRCode::draw(cv::Mat& image) {
        cv::line(image, top, right, cv::Scalar(0, 0, 255), 2);
        cv::line(image, top, bottom, cv::Scalar(0, 255, 0), 2);
        cv::circle(image, center, 5, cv::Scalar(255, 0, 0), -1);
    }

    std::string QRCode::toString() {
        return "[ top=(" + vernier::toString(top.x) + "," + vernier::toString(top.y)
                + "), right=(" + vernier::toString(right.x) + "," + vernier::toString(right.y)
                + "), bottom=(" + vernier::toString(bottom.x) + "," + vernier::toString(bottom.y) + " ]";
    }

    double QRCode::getRadius() {
        int dotCount = (int) (cv::norm(bottom - right)/2.0);
        return dotCount;
    }
    
    void QRCodeDetector::computeDistances() {
        int size = fiducialDetector.fiducials.size();
        distance.resize(size);
        for (int i = 0; i < size; i++) {
            distance[i].resize(size);
        }
        for (int i = 0; i < size; i++) {
            distance[i][i] = 0.0;
            for (int j = i + 1; j < size; j++) {
                distance[i][j] = distance[j][i] = cv::norm(fiducialDetector.fiducials[i].position - fiducialDetector.fiducials[j].position);
            }
        }
    }

    void QRCodeDetector::makeFirstClustering() {
        int clusterCount = fiducialDetector.fiducials.size() / 3;
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
            QRCode code(fiducialDetector.fiducials[clusters[i][0]], fiducialDetector.fiducials[clusters[i][1]], fiducialDetector.fiducials[clusters[i][2]]);

            //double rotationDistance;
            //cv::Point2d refright(code.right.x - code.top.x, code.right.y - code.top.y);
            //cv::Point2d refBottom(code.bottom.x - code.top.x, code.bottom.y - code.top.y);
            //rotationDistance = cv::norm(cv::Point2d(refBottom.y, -refBottom.x) - cv::Point2d(refright.x, refright.y));

            codes.push_back(code);
        }
    }

    void QRCodeDetector::compute(cv::Mat& image) {
        fiducialDetector.compute(image);
        codes.clear();
        if (fiducialDetector.fiducials.size() >= 3) {
            computeDistances();
            makeFirstClustering();
            //refineClusteringUsingKMeans();
            recordCodes();
        }
    }

    void QRCodeDetector::draw(cv::Mat& image) {
        for (int i = 0; i < codes.size(); i++) {
            codes[i].draw(image);
            //cv::putText(image, Vernier::toString(i), codes[i].center, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 4);
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