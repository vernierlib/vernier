/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef GAUSSIANFILTER_HPP
#define GAUSSIANFILTER_HPP

#include "Common.hpp"

namespace Vernier {

    /** \brief Spectral Gaussian filter 
     * 
     * The Gaussian filter allows to select a peak in the spectrum.
     * 
     * All the memory allocations and possible pre-calculations are done at the 
     * construction of the class.
     */
    class GaussianFilter {
    private:

        Eigen::ArrayXXd kernel;
        double sigma;

        void resize(int nRows, int nCols);

    public:

        /** Default constructor (zero filter) */
        GaussianFilter();

        /** Constructs a pre-calculated Gaussian spectral filter.
         *
         * @param sigma: kernel radius
         */
        GaussianFilter(double sigma);

        /** Applies the pre-calculated Gaussian spectral filter on an array.
         *
         * @param array: array to apply the filter
         * @param centerRow: row for filter center
         * @param centerCol: col for filter center
         */
        void applyTo(Eigen::ArrayXXcd& inputArray, int centerRow, int centerCol);

        /** Changes sigma and recalculates the Gaussian spectral filter.
         *
         * @param sigma: kernel radius
         */
        void setSigma(double sigma);

        double getSigma() {
            return sigma;
        }

        Eigen::ArrayXXd getKernel() {
            return kernel;
        }

    };

    /** Apply gaussian spectral filter on an array without any pre-calculation (slower).
     *
     * @param array: array to apply the filter
     * @param centerRow: row for filter center
     * @param centerCol: col for filter center
     * @param sigma: kernel radius
     */
    void gaussianFilter(Eigen::ArrayXXcd& array, double centerRow, double centerCol, double sigma);
}

#endif /* GAUSSIANFILTER_HPP */