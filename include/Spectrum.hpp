/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

#include "Common.hpp"

namespace vernier {

    /** Apply a band pass filter (hard cut).
     * 
     *  \param array: array to apply the filter
     *  \param lowFrequency: minimal frequency
     *  \param highFrequency: maximal frequency
     */
    template<typename _Scalar, int _Rows, int _Cols>
    void applyBandPassCut(Eigen::Array<_Scalar, _Rows, _Cols>& array, double lowFrequency, double highFrequency) {
        ASSERT(array.rows() > 0 && array.cols() > 0);
        int rows = array.rows();
        int cols = array.cols();
        int centerX = rows / 2;
        int centerY = cols / 2;

        for (int col = 0; col < cols; ++col) {
            for (int row = 0; row < rows; ++row) {
                double distance = std::hypot(row - centerX, col - centerY);
                if (distance <= lowFrequency || distance > highFrequency) {
                    array(row, col) = _Scalar();
                }
            }
        }
    }

    /** Remove a angular sector filter (hard cut).
     * 
     *  \param array: array to apply the filter
     *  \param centerAngle: center of the sector to remove (in radians)
     *  \param widthAngle: width of the sector to remove (in radians)
     */
    template<typename _Scalar, int _Rows, int _Cols>
    void applyAngularCut(Eigen::Array<_Scalar, _Rows, _Cols>& array, double centerAngle, double widthAngle) {
        ASSERT(array.rows() > 0 && array.cols() > 0);
        int rows = array.rows();
        int cols = array.cols();
        int centerX = rows / 2;
        int centerY = cols / 2;
        double halfWidthAngle = widthAngle / 2.0;

        for (int col = 0; col < cols; ++col) {
            for (int row = 0; row < rows; ++row) {
                double currentAngle = std::atan2(row - centerX, col - centerY);
                double diff = angleInPiPi(currentAngle - centerAngle);
                if (std::abs(diff) <= halfWidthAngle || std::abs(diff) >= (PI - halfWidthAngle)) {
                    array(row, col) = _Scalar();
                }
            }
        }
    }

    /** Apply gaussian filter on an array
     *
     * @param array: array to apply the filter
     * @param centerRow: row for filter center
     * @param centerCol: col for filter center
     * @param sigma: kernel radius
     */
    template<typename _Scalar, int _Rows, int _Cols>
    void applyGaussianFilter(Eigen::Array<_Scalar, _Rows, _Cols>& array, int centerRow, int centerCol, double sigma) {
        ASSERT(sigma > 0.0);
        double invTwoSigmaSq = 1.0 / (2.0 * sigma * sigma);

        Eigen::ArrayXd rowWeights(array.rows());
        for (int row = 0; row < array.rows(); row++) {
            rowWeights(row) = std::exp(-(row - centerRow) * (row - centerRow) * invTwoSigmaSq);
        }

        Eigen::ArrayXd colWeights(array.cols());
        for (int col = 0; col < array.cols(); col++) {
            colWeights(col) = std::exp(-(col - centerCol) * (col - centerCol) * invTwoSigmaSq);
        }

        for (int j = 0; j < array.cols(); j++) {
            array.col(j) *= rowWeights * colWeights(j);
        }
    }

    /** Shift a complex array to the center.
     *	The algorithm permuts blocks between source and dest (out of place).
     *
     *	\param source: Unshifted array (complex)
     *	\param dest: shifted array (complex)
     */
    void shift(Eigen::ArrayXXcd& source, Eigen::ArrayXXcd& dest);

    /** Shift an array to the center (in-place version).
     */
    template<typename _Scalar, int _Rows, int _Cols>
    void shift(Eigen::Array<_Scalar, _Rows, _Cols>& array) {
        ASSERT((array.rows() % 2 == 0 && array.cols() % 2 == 0)
                || (array.rows() == 1 && array.cols() % 2 == 0)
                || (array.rows() % 2 == 0 && array.cols() == 1));
        for (int col = array.cols() - 1; col >= 0; --col) {
            for (int row = array.rows() - 1; row >= 0; --row) {
                int a = 1 - 2 * ((row + col) & 1);
                array(row, col) *= a;
            }
        }
    }

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
     * 
     * OBSOLETE
     */
    void mainPeakHalfPlane(Eigen::ArrayXXcd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2);

}

#endif // !SPECTRUM_HPP