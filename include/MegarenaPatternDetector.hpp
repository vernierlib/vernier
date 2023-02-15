/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef MEGARENADETECTOR_HPP
#define MEGARENADETECTOR_HPP

#include "PeriodicPatternDetector.hpp"
#include "Thumbnail.hpp"
#include "AbsoluteDecoding.hpp"

namespace Vernier {

    /** \brief Class to estimate the absolute pose of periodic patterns with subpixel resolutions.
     *   This class is based on the positioning of a given __pseudo periodic encoded pattern__.
     *
     * All the memory allocation and possible pre-calculations are done at the
     * construction of the detector, then the images can be computed without any delays
     * (but all the computed images must have the same size).
     *
     */
    class MegarenaPatternDetector : public PeriodicPatternDetector {
    protected:
        Eigen::ArrayXXi bitSequence;
        AbsoluteDecoding decoding;
        int length1, length2;
        Thumbnail thumbnail;
        int codePosition1, codePosition2;
        Plane plane1Save, plane2Save;

        void readJSON(rapidjson::Value& document);

        /** Remove all the "Not A Number" terms of an array
         *
         *   \param image: input image array with NaN to be removed
         *
         **/
        void removeNanFromArray(Eigen::ArrayXXd& image); // To be moved to utils

        void computeAbsolutePose(Eigen::ArrayXXd& pattern);

    public:

        /** Constructs an empty detector for megarena periodic patterns
         *
         */
        MegarenaPatternDetector();

        /** Constructs a detector for megarena periodic patterns with a starting image.
         *
         *	\param image: starting image, opencv format (cv::Mat) used to resize the megarena detector
         *	\param bitSequence: encoded sequence of the megarena pattern
         *	\param approxPixelPeriod: approximative pixelic period as seen on the acquired pattern image
         *	\param physicalPeriod: physical period of the pattern used to build it
         */
#ifdef USE_OPENCV
        MegarenaPatternDetector(cv::Mat image, Eigen::ArrayXXi bitSequence, double approxPixelPeriod, double physicalPeriod = 0);
#endif // USE_OPENCV

        Eigen::Matrix4d getPatternPose();

        /** Constructs a detector for megarena periodic patterns with a starting image.
         *
         *	\param image: starting image, eigen format complex double (Eigen::ArrayXXcd) used to resize the megarena detector
         *	\param bitSequence: encoded sequence of the megarena pattern
         *	\param approxPixelPeriod: approximative pixelic period as seen on the acquired pattern image
         *	\param physicalPeriod: physical period of the pattern used to build it
         */
        MegarenaPatternDetector(Eigen::ArrayXXcd image, Eigen::ArrayXXi bitSequence, double approxPixelPeriod, double physicalPeriod = 0);

        /** Constructs a detector for megarena periodic patterns.
         *
         *	\param bitSequence: encoded sequence of the megarena pattern
         *	\param nRows: number of rows contained in the image to compute
         *	\param nCols: number of columns contained in the image to compute
         *	\param approxPixelPeriod: approximative pixelic period as seen on the acquired pattern image
         *	\param physicalPeriod: physical period of the pattern used to build it
         *	\param sigmaPercent: sigma percentage used to filter the Fourier transform
         */
        MegarenaPatternDetector(Eigen::ArrayXXi bitSequence, int nRows, int nCols, double approxPixelPeriod, double physicalPeriod = 0, double sigmaPercent = 0.005);

        /** Constructs a detector for megarena periodic patterns.
         *
         *	\param codeSize: number of bits of the encoded sequence of the megarena pattern
         *	\param nRows: number of rows contained in the image to compute
         *	\param nCols: number of columns contained in the image to compute
         *	\param approxPixelPeriod: approximative pixelic period as seen on the acquired pattern image
         *	\param physicalPeriod: physical period of the pattern used to build it
         *	\param sigmaPercent: sigma percentage used to filter the Fourier transform
         */
        MegarenaPatternDetector(int codeSize, int nRows, int nCols, double approxPixelPeriod, double physicalPeriod, double sigmaPercent);

        /** Default destructor
         **/
        ~MegarenaPatternDetector() = default;

        /** Resizes the detector for megarena periodic patterns.
         *
         *	\param bitSequence: encoded sequence of the megarena pattern
         *	\param nRows: number of rows contained in the image to compute
         *	\param nCols: number of columns contained in the image to compute
         *	\param approxPixelPeriod: approximative pixelic period as seen on the acquired pattern image
         *	\param physicalPeriod: physical period of the pattern used to build it
         *	\param sigmaPercent: sigma percentage used to filter the Fourier transform
         */
        void resize(Eigen::ArrayXXi bitSequence, int nRows, int nCols, double approxPixelPeriod, double physicalPeriod = 0, double sigmaPercent = 0.005);

        /** Resizes the detector for megarena periodic patterns with a starting image.
         *
         *	\param image: starting image, opencv format (cv::Mat) used to resize the megarena detector
         *	\param bitSequence: encoded sequence of the megarena pattern
         *	\param approxPixelPeriod: approximative pixelic period as seen on the acquired pattern image
         *	\param physicalPeriod: physical period of the pattern used to build it
         */
#ifdef USE_OPENCV
        void resize(cv::Mat image, Eigen::ArrayXXi bitSequence, double approxPixelPeriod, double physicalPeriod = 0);
#endif // USE_OPENCV


        /** Resizes the detector for megarena periodic patterns with a starting image.
         *
         *	\param image: starting image, eigen format complex double (Eigen::ArrayXXcd) used to resize the megarena detector
         *	\param bitSequence: encoded sequence of the megarena pattern
         *	\param approxPixelPeriod: approximative pixelic period as seen on the acquired pattern image
         *	\param physicalPeriod: physical period of the pattern used to build it
         */
        void resize(Eigen::ArrayXXcd image, Eigen::ArrayXXi bitSequence, double approxPixelPeriod, double physicalPeriod = 0);

        /** Resizes the detector main parameters with a given number of rows and columns
         *
         *	\param nRows: number of rows contained in the image to compute
         *	\param nCols: number of columns contained in the image to compute
         */
        void resize(int nRows, int nCols);

        /** Returns the computed thumbnail of the image given to the megarena detector
         *
         */
        Thumbnail getThumbnail();

        // #ifdef USE_OPENCV
        // 		cv::Mat getGuiSpectrum();
        // #endif // USE_OPENCV

        void compute(Eigen::ArrayXXd& pattern);

#ifdef USE_OPENCV
        void computePerspective(Eigen::ArrayXXd& pattern);
#endif // USE_OPENCV

        Pose get2DPose();

        std::vector<Pose> getAll3DPoses();
        
        void showControlImages();
        
        int getInt(const std::string & attribute);

        void* getObject(const std::string & attribute);

    };
}

#endif