/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef FOURIERTRANSFORM_H
#define FOURIERTRANSFORM_H

#include "Common.hpp"

namespace vernier {

    /** \brief Computes Discrete Fourier Transform on Eigen arrays using pocketfft.
     *
     * Both directions are unnormalized: a forward then backward transform scales
     * the data by the number of elements.
     */
    class FourierTransform {
    public:

        enum Direction {
            FORWARD = -1,
            BACKWARD = 1
        };

        /** Constructs the transform for a given size
         *
         * \param sign: FORWARD (default) or BACKWARD
         */
        FourierTransform(int sign = FORWARD);

        /** Constructs the transform for a given size
         *
         * \param nRows: number of rows of the array
         * \param nCols: number of cols of the array
         * \param sign: FORWARD or BACKWARD
         */
        FourierTransform(int nRows, int nCols = 1, int sign = FORWARD);

        /** Constructs the transform for the size of an array
         *
         * \param array: 2-D complex array (only the size of array is used, no
         * transformation is computed at this step)
         * \param sign: FORWARD or BACKWARD
         */
        FourierTransform(Eigen::ArrayXXcd& array, int sign = FORWARD);

        /** Constructs the transform for the size of an array
         *
         * \param array: 1-D complex array (only the size of array is used, no
         * transformation is computed at this step)
         * \param sign: FORWARD or BACKWARD
         */
        FourierTransform(Eigen::ArrayXcd& array, int sign = FORWARD);

        /** Resizes the transform
         *
         * \param nRows: number of rows of the array
         * \param nCols: number of cols of the array
         * \param sign: FORWARD or BACKWARD
         */
        void resize(int nRows, int nCols, int sign = FORWARD);

        /** Computes the transform
         *
         * \param in: 2-D complex input array
         * \param out: 2-D complex output array
         */
        void compute(const Eigen::ArrayXXcd& in, Eigen::ArrayXXcd& out);

        /** Computes the transform
         *
         * \param in: 1-D complex input array
         * \param out: 1-D complex output array
         */
        void compute(const Eigen::ArrayXcd& in, Eigen::ArrayXcd& out);
        
        /** Set the direction of the FFT
         *
         * \param sign: FORWARD or BACKWARD
         */
        void setSign(int sign);

    protected:

        int nRows;
        int nCols;
        int sign;
    };
}

#endif