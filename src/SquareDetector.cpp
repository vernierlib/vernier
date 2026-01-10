/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UMLP.
 */

#include "SquareDetector.hpp"

namespace vernier {

// Square detector extracted from aruco_detector.cpp of OpenCV 4.13.0 ----------

    using namespace std;
    using namespace cv;
    //using namespace cv::aruco;

    /**
     * @brief Given a tresholded image, find the contours, calculate their polygonal approximation
     * and take those that accomplish some conditions
     */
    static void _findMarkerContours(const Mat &in, vector<vector<Point2f> > &candidates,
            vector<vector<Point> > &contoursOut, double minPerimeterRate,
            double maxPerimeterRate, double accuracyRate,
            double minCornerDistanceRate, int minDistanceToBorder, int minSize) {

        CV_Assert(minPerimeterRate > 0 && maxPerimeterRate > 0 && accuracyRate > 0 &&
                minCornerDistanceRate >= 0 && minDistanceToBorder >= 0);

        // calculate maximum and minimum sizes in pixels
        unsigned int minPerimeterPixels =
                (unsigned int) (minPerimeterRate * max(in.cols, in.rows));
        unsigned int maxPerimeterPixels =
                (unsigned int) (maxPerimeterRate * max(in.cols, in.rows));

        // for aruco3 functionality
        if (minSize != 0) {
            minPerimeterPixels = 4 * minSize;
        }

        vector<vector<Point> > contours;
        findContours(in, contours, RETR_LIST, CHAIN_APPROX_NONE);
        // now filter list of contours
        for (unsigned int i = 0; i < contours.size(); i++) {
            // check perimeter
            if (contours[i].size() < minPerimeterPixels || contours[i].size() > maxPerimeterPixels)
                continue;

            // check is square and is convex
            vector<Point> approxCurve;
            approxPolyDP(contours[i], approxCurve, double(contours[i].size()) * accuracyRate, true);
            if (approxCurve.size() != 4 || !isContourConvex(approxCurve)) continue;

            // check min distance between corners
            double minDistSq = max(in.cols, in.rows) * max(in.cols, in.rows);
            for (int j = 0; j < 4; j++) {
                double d = (double) (approxCurve[j].x - approxCurve[(j + 1) % 4].x) *
                        (double) (approxCurve[j].x - approxCurve[(j + 1) % 4].x) +
                        (double) (approxCurve[j].y - approxCurve[(j + 1) % 4].y) *
                        (double) (approxCurve[j].y - approxCurve[(j + 1) % 4].y);
                minDistSq = min(minDistSq, d);
            }
            double minCornerDistancePixels = double(contours[i].size()) * minCornerDistanceRate;
            if (minDistSq < minCornerDistancePixels * minCornerDistancePixels) continue;

            // check if it is too near to the image border
            bool tooNearBorder = false;
            for (int j = 0; j < 4; j++) {
                if (approxCurve[j].x < minDistanceToBorder || approxCurve[j].y < minDistanceToBorder ||
                        approxCurve[j].x > in.cols - 1 - minDistanceToBorder ||
                        approxCurve[j].y > in.rows - 1 - minDistanceToBorder)
                    tooNearBorder = true;
            }
            if (tooNearBorder) continue;

            // if it passes all the test, add to candidates vector
            vector<Point2f> currentCandidate;
            currentCandidate.resize(4);
            for (int j = 0; j < 4; j++) {
                currentCandidate[j] = Point2f((float) approxCurve[j].x, (float) approxCurve[j].y);
            }
            candidates.push_back(currentCandidate);
            contoursOut.push_back(contours[i]);
        }
    }

    /**
     * @brief Threshold input image using adaptive thresholding
     */
    static void _threshold(InputArray _in, OutputArray _out, int winSize, double constant) {

        CV_Assert(winSize >= 3);
        if (winSize % 2 == 0) winSize++; // win size must be odd
        adaptiveThreshold(_in, _out, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, winSize, constant);
    }

    /**
     * @brief Initial steps on finding square candidates
     */
    static void _detectInitialCandidates(const Mat &grey, vector<vector<Point2f> > &candidates,
            vector<vector<Point> > &contours,
            const SquareDetectorParameters &params) {

        CV_Assert(params.adaptiveThreshWinSizeMin >= 3 && params.adaptiveThreshWinSizeMax >= 3);
        CV_Assert(params.adaptiveThreshWinSizeMax >= params.adaptiveThreshWinSizeMin);
        CV_Assert(params.adaptiveThreshWinSizeStep > 0);

        // number of window sizes (scales) to apply adaptive thresholding
        int nScales = (params.adaptiveThreshWinSizeMax - params.adaptiveThreshWinSizeMin) /
                params.adaptiveThreshWinSizeStep + 1;

        vector<vector<vector<Point2f> > > candidatesArrays((size_t) nScales);
        vector<vector<vector<Point> > > contoursArrays((size_t) nScales);

        ////for each value in the interval of thresholding window sizes
        parallel_for_(Range(0, nScales), [&](const Range & range) {
            const int begin = range.start;
            const int end = range.end;

            for (int i = begin; i < end; i++) {
                int currScale = params.adaptiveThreshWinSizeMin + i * params.adaptiveThreshWinSizeStep;
                        // threshold
                        Mat thresh;
                        _threshold(grey, thresh, currScale, params.adaptiveThreshConstant);

                        // detect rectangles
                        _findMarkerContours(thresh, candidatesArrays[i], contoursArrays[i],
                        params.minMarkerPerimeterRate, params.maxMarkerPerimeterRate,
                        params.polygonalApproxAccuracyRate, params.minCornerDistanceRate,
                        params.minDistanceToBorder, params.minSideLengthCanonicalImg);
            }
        });
        // join candidates
        for (int i = 0; i < nScales; i++) {
            for (unsigned int j = 0; j < candidatesArrays[i].size(); j++) {
                candidates.push_back(candidatesArrays[i][j]);
                contours.push_back(contoursArrays[i][j]);
            }
        }
    }

    /**
     * @brief Assure order of candidate corners is clockwise direction
     */
    static void _reorderCandidatesCorners(vector<vector<Point2f> > &candidates) {

        for (unsigned int i = 0; i < candidates.size(); i++) {
            double dx1 = candidates[i][1].x - candidates[i][0].x;
            double dy1 = candidates[i][1].y - candidates[i][0].y;
            double dx2 = candidates[i][2].x - candidates[i][0].x;
            double dy2 = candidates[i][2].y - candidates[i][0].y;
            double crossProduct = (dx1 * dy2) - (dy1 * dx2);

            if (crossProduct < 0.0) { // not clockwise direction
                swap(candidates[i][1], candidates[i][3]);
            }
        }
    }

    static float getAverageModuleSize(const vector<Point2f>& markerCorners, int markerSize, int markerBorderBits) {
        float averageArucoModuleSize = 0.f;
        for (size_t i = 0ull; i < 4ull; i++) {
            averageArucoModuleSize += sqrt(normL2Sqr<float>(Point2f(markerCorners[i] - markerCorners[(i + 1ull) % 4ull])));
        }
        int numModules = markerSize + markerBorderBits * 2;
        averageArucoModuleSize /= ((float) markerCorners.size() * numModules);
        return averageArucoModuleSize;
    }

    static bool checkMarker1InMarker2(const vector<Point2f>& marker1, const vector<Point2f>& marker2) {
        return pointPolygonTest(marker2, marker1[0], false) >= 0 && pointPolygonTest(marker2, marker1[1], false) >= 0 &&
                pointPolygonTest(marker2, marker1[2], false) >= 0 && pointPolygonTest(marker2, marker1[3], false) >= 0;
    }

    struct MarkerCandidate {
        vector<Point2f> corners;
        vector<Point> contour;
        float perimeter = 0.f;
    };

    struct MarkerCandidateTree : MarkerCandidate {
        int parent = -1;
        int depth = 0;
        vector<MarkerCandidate> closeContours;

        MarkerCandidateTree() {
        }

        MarkerCandidateTree(vector<Point2f>&& corners_, vector<Point>&& contour_) {
            corners = std::move(corners_);
            contour = std::move(contour_);
            perimeter = 0.f;
            for (size_t i = 0ull; i < 4ull; i++) {
                perimeter += sqrt(normL2Sqr<float>(Point2f(corners[i] - corners[(i + 1ull) % 4ull])));
            }
        }

        bool operator<(const MarkerCandidateTree& m) const {
            // sorting the contors in descending order
            return perimeter > m.perimeter;
        }
    };

    // returns the average distance between the marker points

    float static inline getAverageDistance(const std::vector<Point2f>& marker1, const std::vector<Point2f>& marker2) {
        float minDistSq = std::numeric_limits<float>::max();
        // fc is the first corner considered on one of the markers, 4 combinations are possible
        for (int fc = 0; fc < 4; fc++) {
            float distSq = 0;
            for (int c = 0; c < 4; c++) {
                // modC is the corner considering first corner is fc
                int modC = (c + fc) % 4;
                distSq += normL2Sqr<float>(marker1[modC] - marker2[c]);
            }
            distSq /= 4.f;
            minDistSq = min(minDistSq, distSq);
        }
        return sqrt(minDistSq);
    }

    struct ArucoDetectorImpl {
        /// dictionary indicates the type of markers that will be searched
        //Dictionary dictionary;

        /// marker detection parameters, check SquareDetectorParameters docs to see available settings
        SquareDetectorParameters detectorParams;

        /// marker refine parameters
        //RefineParameters refineParams;

        ArucoDetectorImpl() {
        }

//        ArucoDetectorImpl(const Dictionary &_dictionary, const DetectorParameters &_detectorParams,
//                const RefineParameters& _refineParams) : dictionary(_dictionary),
//        detectorParams(_detectorParams), refineParams(_refineParams) {
//        }
        
        ArucoDetectorImpl(const SquareDetectorParameters &_detectorParams) : detectorParams(_detectorParams) {
        }

        /**
         * @brief Detect square candidates in the input image
         */
        void detectCandidates(const Mat& grey, vector<vector<Point2f> >& candidates, vector<vector<Point> >& contours) {
            /// 1. DETECT FIRST SET OF CANDIDATES
            _detectInitialCandidates(grey, candidates, contours, detectorParams);
            /// 2. SORT CORNERS
            _reorderCandidatesCorners(candidates);
        }

        /**
         * @brief  FILTER OUT NEAR CANDIDATE PAIRS
         *
         * save the outter/inner border (i.e. potential candidates) to vector<MarkerCandidateTree>,
         * clear candidates and contours
         */
        vector<MarkerCandidateTree>
        filterTooCloseCandidates(vector<vector<Point2f> > &candidates, vector<vector<Point> > &contours) {
            CV_Assert(detectorParams.minMarkerDistanceRate >= 0.);
            vector<MarkerCandidateTree> candidateTree(candidates.size());
            for (size_t i = 0ull; i < candidates.size(); i++) {
                candidateTree[i] = MarkerCandidateTree(std::move(candidates[i]), std::move(contours[i]));
            }
            candidates.clear();
            contours.clear();

            // sort candidates from big to small
            std::stable_sort(candidateTree.begin(), candidateTree.end());
            // group index for each candidate
            vector<int> groupId(candidateTree.size(), -1);
            vector<vector<size_t> > groupedCandidates;
            vector<bool> isSelectedContours(candidateTree.size(), true);

            size_t countSelectedContours = 0ull;
            for (size_t i = 0ull; i < candidateTree.size(); i++) {
                for (size_t j = i + 1ull; j < candidateTree.size(); j++) {
                    float minDist = getAverageDistance(candidateTree[i].corners, candidateTree[j].corners);
                    // if mean distance is too low, group markers
                    // the distance between the points of two independent markers should be more than half the side of the marker
                    // half the side of the marker = (perimeter / 4) * 0.5 = perimeter * 0.125
                    if (minDist < candidateTree[j].perimeter * (float) detectorParams.minMarkerDistanceRate) {
                        isSelectedContours[i] = false;
                        isSelectedContours[j] = false;
                        // i and j are not related to a group
                        if (groupId[i] < 0 && groupId[j] < 0) {
                            // mark candidates with their corresponding group number
                            groupId[i] = groupId[j] = (int) groupedCandidates.size();
                            // create group
                            groupedCandidates.push_back({i, j});
                        }// i is related to a group
                        else if (groupId[i] > -1 && groupId[j] == -1) {
                            int group = groupId[i];
                            groupId[j] = group;
                            // add to group
                            groupedCandidates[group].push_back(j);
                        }// j is related to a group
                        else if (groupId[j] > -1 && groupId[i] == -1) {
                            int group = groupId[j];
                            groupId[i] = group;
                            // add to group
                            groupedCandidates[group].push_back(i);
                        }
                    }
                }
                countSelectedContours += isSelectedContours[i];
            }

            for (vector<size_t>& grouped : groupedCandidates) {
                if (detectorParams.detectInvertedMarker) // if detectInvertedMarker choose smallest contours
                    std::stable_sort(grouped.begin(), grouped.end(), [](const size_t & a, const size_t & b) {
                        return a > b;
                    });
                else // if detectInvertedMarker==false choose largest contours
                    std::stable_sort(grouped.begin(), grouped.end());
                size_t currId = grouped[0];
                isSelectedContours[currId] = true;
                for (size_t i = 1ull; i < grouped.size(); i++) {
                    size_t id = grouped[i];
                    float dist = getAverageDistance(candidateTree[id].corners, candidateTree[currId].corners);
                    //float moduleSize = getAverageModuleSize(candidateTree[id].corners, dictionary.markerSize, detectorParams.markerBorderBits);
                    float moduleSize = getAverageModuleSize(candidateTree[id].corners, detectorParams.markerSize, detectorParams.markerBorderBits);
                    if (dist > detectorParams.minGroupDistance * moduleSize) {
                        currId = id;
                        candidateTree[grouped[0]].closeContours.push_back(candidateTree[id]);
                    }
                }
            }

            vector<MarkerCandidateTree> selectedCandidates(countSelectedContours + groupedCandidates.size());
            countSelectedContours = 0ull;
            for (size_t i = 0ull; i < candidateTree.size(); i++) {
                if (isSelectedContours[i]) {
                    selectedCandidates[countSelectedContours] = std::move(candidateTree[i]);
                    countSelectedContours++;
                }
            }

            // find hierarchy in the candidate tree
            for (int i = (int) selectedCandidates.size() - 1; i >= 0; i--) {
                for (int j = i - 1; j >= 0; j--) {
                    if (checkMarker1InMarker2(selectedCandidates[i].corners, selectedCandidates[j].corners)) {
                        selectedCandidates[i].parent = j;
                        selectedCandidates[j].depth = max(selectedCandidates[j].depth, selectedCandidates[i].depth + 1);
                        break;
                    }
                }
            }
            return selectedCandidates;
        }
    };

//    // Derived class to expose arucoDetectorImpl that is protected
//    class CustomArucoDetector : public ArucoDetector {
//    public:
//        //using cv::aruco::ArucoDetector::ArucoDetector; // Hérite du constructeur
//
//        CustomArucoDetector(const Dictionary &dictionary = getPredefinedDictionary(cv::aruco::DICT_4X4_50),
//                const DetectorParameters &detectorParams = DetectorParameters(),
//                const RefineParameters& refineParams = RefineParameters()) {
//            arucoDetectorImpl = cv::makePtr<ArucoDetectorImpl>(dictionary, detectorParams, refineParams);
//        }
//
//        Ptr<ArucoDetectorImpl> get() {
//            return arucoDetectorImpl;
//        }
//
//    };

//------------------------------------------------------------------------------

    Square::Square(cv::Point2f corner0, cv::Point2f corner1, cv::Point2f corner2, cv::Point2f corner3) {
        topLeft = corner0;
        topRight = corner1;
        bottomRight = corner2;
        bottomLeft = corner3;
    }

    Square::Square(std::vector<cv::Point2f> corners) {
        ASSERT(corners.size() == 4);
        topLeft = corners[0];
        topRight = corners[1];
        bottomRight = corners[2];
        bottomLeft = corners[3];
    }

    cv::Point2f Square::getCenter() {
        return (topRight + topLeft + bottomRight + bottomLeft) / 4.0;
    }

    double Square::getAngle() {
        cv::Point2f direction = ((topRight - topLeft) + (bottomRight - bottomLeft)) / 2.0;
        return atan2(direction.y, direction.x);
    }

    void Square::draw(cv::Mat& image, cv::Scalar color) {
        cv::line(image, topLeft, topRight, color, 2);
        cv::line(image, topLeft, bottomLeft, color, 2);
        cv::line(image, topRight, bottomRight, color, 2);
        cv::line(image, bottomLeft, bottomRight, color, 2);
    }

    double Square::getRadius() {
        double diagonal1 = cv::norm(topLeft - bottomRight);
        double diagonal2 = cv::norm(topRight - bottomLeft);
        return std::max(diagonal1, diagonal2) / 2.0;
    }

    std::string Square::toString() {
        return "[ topLeft=(" + vernier::to_string(topLeft.x) + "," + vernier::to_string(topLeft.y)
                + "), topRight=(" + vernier::to_string(topRight.x) + "," + vernier::to_string(topRight.y)
                + "), bottomRight=(" + vernier::to_string(bottomRight.x) + "," + vernier::to_string(bottomRight.y)
                + "), bottomLeft=(" + vernier::to_string(bottomLeft.x) + "," + vernier::to_string(bottomLeft.y) + " ]";
    }

    SquareDetector::SquareDetector() {
        parameters = SquareDetectorParameters();
        
    }

    void SquareDetector::compute(const cv::Mat& image) {
        //arucoDetectorImpl = cv::makePtr<ArucoDetectorImpl>(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250), parameters, cv::aruco::RefineParameters());
        //Ptr<ArucoDetectorImpl> arucoDetectorImpl = cv::makePtr<ArucoDetectorImpl>(parameters);
        ArucoDetectorImpl arucoDetectorImpl = ArucoDetectorImpl(parameters);
        
        std::vector<std::vector<cv::Point> > contours;
        std::vector<std::vector<cv::Point2f> > candidates;
        arucoDetectorImpl.detectCandidates(image, candidates, contours);

        std::vector<MarkerCandidateTree> selectedCandidates;
        selectedCandidates = arucoDetectorImpl.filterTooCloseCandidates(candidates, contours);

        squares.clear();
        for (int i = 0; i < selectedCandidates.size(); i++) {
            if (selectedCandidates[i].parent < 0) {
                Square s(selectedCandidates[i].corners);
                squares.push_back(s);
            }
        }
    }

    void SquareDetector::draw(cv::Mat& image) {
        for (int i = 0; i < squares.size(); i++) {
            squares[i].draw(image);
        }
    }

    std::string SquareDetector::toString() {
        std::string result = "{ ";
        for (int i = 0; i < squares.size(); i++) {
            result += squares[i].toString() + " ; ";
        }
        result += " }";
        return result;
    }

}