/*
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef BITMAPPATTERNDETECTOR_HPP
#define BITMAPPATTERNDETECTOR_HPP

#include "PeriodicPatternDetector.hpp"
#include "Thumbnail.hpp"

namespace vernier {

    /** \brief Class to estimate the absolute pose of periodic patterns with subpixel resolutions.
     *   This class is based on the positioning of a given __bitmap template periodic pattern__.
     *
     * All the memory allocation and possible pre-calculations are done at the
     * construction of the detector, then the images can be computed without any delays
     * (but all the computed images must have the same size).
     *
     */
    class BitmapPatternDetector : public PeriodicPatternDetector {
    protected:
        Eigen::ArrayXXcd pattern, snapshot;
        int wTemplate, hTemplate;
        Thumbnail thumbnail;
        Pose posePattern;
        Eigen::ArrayXXd thumbnailArray, templatePrime, imagePrime, resultArray;
        double xPattern, yPattern;
        int centerRow, centerCol;
        int length1, length2;
        double phiRoughX, phiRoughY;
        double phiOffset1, phiOffset2;
        int quarter;
        bool isSymetric, isInverted;
        int thumbLength1, thumbLength2;

        /** Takes a snapchot of an overall image to keep only a rectangle of interest centered around the periodic pattern bitmap 
         *
         *   \param snapshot: global image to be cropped
         *   \param radius: radius in pixels of the snapshot
         *   \param xCenter: X center of the snapshot in the global image
         *   \param yCenter: Y center of the snapshot in the global image
         *
         **/
        void takeSnapshot(Eigen::ArrayXXcd &snapshot, double radius, double xCenter, double yCenter);

        /** Get the center of the bitmap based on the thumbnail and the mask applied to retrieve this center.
         *
         *   \param thumbnailArray: image thumbnail
         *   \param bitmapKernel: mask applied to retrieve the center of the bitmal in the thumbnail
         *   \param centerRow: Y position of the bitmap kernel center in the all thumbnail
         *   \param centerCol: X position of the bitmap kernel center in the all thumbnail
         *
         **/
        int getThumbnailCenter(Eigen::ArrayXXd& thumbnailArray, Eigen::ArrayXXd &bitmapKernel, int &centerRow, int &centerCol);

        /** Rotates a kernel around its center of 90 degrees.
         *
         *   \param bitmapKernel: kernel of the bitmap to rotate
         *
         **/
        void rotate90deg(Eigen::ArrayXXd& bitmapKernel);

        void padKernel();

        /** Reads the parameters involved in the positioning of a bitmap periodic pattern in an image
         *
         *   \param document: Rapidjson document where the important information regarding the positioning of a bitmap periodic pattern lies
         *
         **/
        void readJSON(rapidjson::Value& document);

        void computeAbsolutePose(Eigen::ArrayXXd& patternImage);

    public:
        Eigen::ArrayXXd bitmapKernel;

        /** Constructs an empty detector for bitmap periodic patterns
         *
         */
        BitmapPatternDetector();


        /** Constructs a detector for bitmap periodic patterns
         *
         * \param bitmapKernel: bitmap template of the considered periodic pattern
         * \param approxPixelPeriod: approximate length of one period in pixels
         * \param physicalPeriod: real length of one period
         */
        BitmapPatternDetector(Eigen::ArrayXXd bitmapKernel, double approxPixelPeriod, double physicalPeriod = 0);

        /** Constructs a detector for bitmap periodic patterns with a Json file
         *
         * \param filename: name of the Json file containing all the relevant information concerning the considered pattern
         * \param approxPixelPeriod: approximate length of one period in pixels
         */
        BitmapPatternDetector(std::string filename, double approxPixelPeriod);

        /** Resizes the detector
         *
         * \param bitmapKernel: bitmap template of the considered periodic pattern
         * \param approxPixelPeriod: approximate length of one period in pixels
         * \param physicalPeriod: real length of one period
         */
        void resize(Eigen::ArrayXXd bitmapKernel, double approxPixelPeriod, double physicalPeriod);


        /** Resizes the detector
         *
         * \param filename: name of the Json file containing all the relevant information concerning the considered pattern
         * \param approxPixelPeriod: approximate length of one period in pixels
         */
        void resize(std::string filename, double approxPixelPeriod);

        /** Resizes the detector
         *
         * \param nRows: number of rows of the image
         * \param nCols: number of cols of the image
         */
        void resize(int nRows, int nCols);

        /** Estimate the absolute pose of the pattern for a given Eigen array (with an orthographic projection)
         *
         * \param pattern: image of the pattern
         */
        void compute(Eigen::ArrayXXd & pattern);


        /** Estimate the absolute pose of the pattern for a given Eigen array when a perspective projection is considered.
         *
         * \param pattern: image of the pattern
         */
        void computePerspective(Eigen::ArrayXXd& pattern);

        /** Returns the absolute pose of the camera inside the pattern frame.
         *  Please refer to the documentation datasheet concerning the method of absolute positioning
         *
         * The units of lengthes are the same than the one of the physical period.
         */
        Pose getCameraPoseInPattern();

        /** Returns the absolute pose of the pattern inside the camera frame.
         *  Please refer to the documentation datasheet concerning the method of absolute positioning
         *
         * The units of lengthes are the same than the one of the physical period.
         */
        Pose getPatternPoseInCamera();

        std::vector<Pose> getAll3DPoses(int id = 0);

        /** Returns the first direction computed phase plane of the bitmap periodic pattern in the image
         **/
        Plane getPlane1();

        /** Returns the second direction computed phase plane of the bitmap periodic pattern in the image
         **/
        Plane getPlane2();

        void guiDisplayThumbnail(cv::Mat& thumbnailImage);

    };
}
#endif