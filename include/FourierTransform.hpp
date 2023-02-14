/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef FOURIERTRANSFORM_H
#define FOURIERTRANSFORM_H

#include "Common.hpp"

#ifdef USE_FFTW
#include <fftw3.h>
#else
#define FFTW_FORWARD 1
#define FFTW_BACKWARD -1
#endif

namespace Vernier {

    /** \brief Computes Discrete Fourier Transform on Eigen arrays using FFTW library
     * or Ooura's implementation if FFTW is not available.
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
        void compute(Eigen::ArrayXXcd& in, Eigen::ArrayXXcd& out);

        /** Computes the transform using prepared FFT plan
         *
         * \param in: 1-D complex input array
         * \param out: 1-D complex output array
         */
        void compute(Eigen::ArrayXcd& in, Eigen::ArrayXcd& out);
        
        /** Set the direction of the FFT
         *
         * \param sign: FFTW_FORWARD or FFTW_BACKWARD
         */
        void setSign(int sign);

    protected:

        int nRows;
        int nCols;
        int sign;

#ifdef USE_FFTW
        fftw_plan plan;
#else
        double** data;
        double* workArea;
        int* bitReversal;
        double* cosSinTable;
#endif
    };
}

#endif