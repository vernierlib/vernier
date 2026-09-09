/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef FOURIERTRANSFORM_H
#define FOURIERTRANSFORM_H

#include "Common.hpp"

#include <fftw3.h>

namespace vernier {

    /** \brief Computes Discrete Fourier Transform on Eigen arrays using FFTW library.
     *
     * FFT plans are prepared at the construction of the object, then the transforms 
     * can be computed without any delays.
     */
    class FourierTransform {
    public:

        /** Constructs the FFT plans for a given size
         *
         * \param sign: FFTW_FORWARD (default) or FFTW_BACKWARD
         */
        FourierTransform(int sign = FFTW_FORWARD);

        /** Constructs the FFT plans for a given size
         *
         * \param nRows: number of rows of the array
         * \param nCols: number of cols of the array
         * \param sign: FFTW_FORWARD or FFTW_BACKWARD
         */
        FourierTransform(int nRows, int nCols = 1, int sign = FFTW_FORWARD);

        /** Constructs the FFT plans for the size of an array
         *
         * \param array: 2-D complex array (only the size of array is used, no
         * transformation is computed at this step)
         * \param sign: FFTW_FORWARD or FFTW_BACKWARD
         */
        FourierTransform(Eigen::ArrayXXcd& array, int sign = FFTW_FORWARD);

        /** Constructs the FFT plans for the size of an array
         *
         * \param array: 1-D complex array (only the size of array is used, no
         * transformation is computed at this step)
         * \param sign: FFTW_FORWARD or FFTW_BACKWARD
         */
        FourierTransform(Eigen::ArrayXcd& array, int sign = FFTW_FORWARD);

        /** Copying is disabled since the instance owns the FFT plans */
        FourierTransform(const FourierTransform&) = delete;

        FourierTransform& operator=(const FourierTransform&) = delete;

        /** Moving transfers ownership of the FFT plans */
        FourierTransform(FourierTransform&&) noexcept;

        FourierTransform& operator=(FourierTransform&&) noexcept;

        ~FourierTransform();

        /** Resizes the FFT plans
         *
         * \param nRows: number of rows of the array
         * \param nCols: number of cols of the array
         * \param sign: FFTW_FORWARD or FFTW_BACKWARD
         */
        void resize(int nRows, int nCols, int sign = FFTW_FORWARD);

        /** Computes the transform using prepared FFT plan
         *
         * \param in: 2-D complex input array
         * \param out: 2-D complex output array
         */
        void compute(const Eigen::ArrayXXcd& in, Eigen::ArrayXXcd& out);

        /** Computes the transform using prepared FFT plan
         *
         * \param in: 1-D complex input array
         * \param out: 1-D complex output array
         */
        void compute(const Eigen::ArrayXcd& in, Eigen::ArrayXcd& out);
        
        /** Set the direction of the FFT
         *
         * \param sign: FFTW_FORWARD or FFTW_BACKWARD
         */
        void setSign(int sign);

    protected:

        int nRows;
        int nCols;
        int sign;

        fftw_plan plan;
    };
}

#endif