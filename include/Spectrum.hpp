/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

#include "Common.hpp"

namespace vernier {

    /** \brief Computes analysis on the spectrum mainly in input for the following methods.
     *
     * The example below shows how to use this class.
     */
    class Spectrum {
    public:

        /** Shift a complex array to the center.
         *	The algorithm permuts blocks
         *
         *	\param source: Unshifted array (complex)
         *	\param dest: shifted array (complex)
         */
        static void shift(Eigen::ArrayXXcd& source, Eigen::ArrayXXcd& dest);

        /** Search the main peak of the spectrum to prepare for the inverse Fourier transfrom
         *	and where to apply the hypergaussian filter
         *
         *	This function works better when the approximate pixelic period of the pattern is known
         *	and when small off-axis angles are considered (below $3\pi/8$).
         *
         *	\param source: complex array where the search is made
         *	\param mainPeak1: vector containing one local maximum (located in one quadrant of the spectrum)
         *	\param mainPeak2: vector containing the second local maximum (located in the next quadrant of the spectrum)
         *	\param approxPixelPeriod: approximate pixelic period of the pattern, the search of the two maximums is bound in a $[\lambda_{px} - \sqrt{2}\cdot \lambda_{px}; \lambda_{px} + \sqrt{2}\cdot \lambda_{px}]$ interval
         */
        static void mainPeakCircle(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2, double approxPixelPeriod);


        /** Search the main peak of the spectrum to prepare for the inverse Fourier transfrom
         *	and where to apply the hypergaussian filter
         *
         *	This function is aimed to work when no approximate pixelic period is known before the computation.
         *	REPLACED BY mainPeakPerimeter() AND mainPeak4Search()
         *
         *	\param source: complex array where the search is made
         *	\param mainPeak1: vector containing one local maximum (located in one quadrant of the spectrum)
         *	\param mainPeak2: vector containing the second local maximum (located in the next quadrant of the spectrum)
         */
        static void mainPeakQuarter(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2);


        /** Search the main peak of the spectrum to prepare for the inverse Fourier transfrom
         *	and where to apply the hypergaussian filter
         *
         *	Documentation tbd
         *
         *	\param source: complex array where the search is made
         *	\param mainPeak1: vector containing one local maximum (located in one quadrant of the spectrum)
         *	\param mainPeak2: vector containing the second local maximum (located in the next quadrant of the spectrum)
         */
        static void mainPeakPerimeter(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2);


        /** Search the main peak of the spectrum to prepare for the inverse Fourier transfrom
         *	and where to apply the hypergaussian filter
         *
         *	This function is aimed to work when no approximate pixelic period is known before the computation.
         *	REPLACED BY mainPeakPerimeter() AND mainPeak4Search()
         *
         *	\param source: complex array where the search is made
         *	\param mainPeak: vector containing one local maximum (located in one quadrant of the spectrum)
         *	\param quarter: quarter where to search the maximum
         *	\param frequencyMin: to restrain the search, we only make it on a ring. frequencyMin = cols/pixelPeriodMin
         *	\param frequencyMin: same as frequencyMin, except frequencyMax = cols/pixelPeriodMax
         *	\param frequencyMin: lower bound of the spectral ring to search the main peak of the spectrum
         *	\param frequencyMax: upper bound of the spectral ring to search the main peak of the spectrum
         */
        static void mainPeakHalfPlane(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2);


        /** Search the main peak of the spectrum to prepare for the inverse Fourier transfrom
         *	and where to apply the hypergaussian filter
         *
         *	Documentation tbd
         *
         *	\param source: complex array where the search is made
         *	\param mainPeak1: vector containing one local maximum (located in one quadrant of the spectrum)
         *	\param mainPeak2: vector containing the second local maximum (located in the next quadrant of the spectrum)
         */
        static void mainPeak4Search(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2);

        static double getDistancePoints(double x1, double y1, double x2, double y2);


    };
}

#endif // !SPECTRUM_HPP