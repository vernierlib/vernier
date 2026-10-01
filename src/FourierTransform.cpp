/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "FourierTransform.hpp"

#define POCKETFFT_CACHE_SIZE 16
#if defined(__unix__) || defined(__APPLE__)
#define POCKETFFT_PTHREADS
#endif
#include <pocketfft/pocketfft_hdronly.h>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace vernier {

    FourierTransform::FourierTransform(int sign) {
        nRows = 0;
        nCols = 0;
        setSign(sign);
    }

    FourierTransform::FourierTransform(int rows, int cols, int sign) : FourierTransform() {
        resize(rows, cols, sign);
    }

    FourierTransform::FourierTransform(Eigen::ArrayXXcd& array, int sign) : FourierTransform() {
        resize(array.rows(), array.cols(), sign);
    }

    FourierTransform::FourierTransform(Eigen::ArrayXcd& array, int sign) : FourierTransform() {
        resize(array.rows(), array.cols(), sign);
    }

    void FourierTransform::resize(int nRows, int nCols, int sign) {
        if (nRows <= 0 || nCols <= 0) {
            throw Exception("Can't resize a FourierTransform with rows<=0 or cols<=0");
        }
        this->nRows = nRows;
        this->nCols = nCols;
        setSign(sign);
    }

    static void transform(const std::complex<double>* in, std::complex<double>* out, int nRows, int nCols, int sign) {
        // Eigen arrays are column-major, so the data is a row-major nCols x nRows array
        pocketfft::shape_t shape{(size_t) nCols, (size_t) nRows};
        pocketfft::stride_t stride{(ptrdiff_t) (nRows * sizeof (std::complex<double>)), (ptrdiff_t) sizeof (std::complex<double>)};
        pocketfft::shape_t axes{0, 1};
        // 0 lets pocketfft use every core, unless the caller already runs threads through OpenMP
        size_t nThreads = 0;
#ifdef _OPENMP
        if (omp_in_parallel()) {
            nThreads = 1;
        }
#endif
        pocketfft::c2c(shape, stride, stride, axes, sign == FourierTransform::FORWARD, in, out, 1.0, nThreads);
    }

    void FourierTransform::compute(const Eigen::ArrayXXcd& in, Eigen::ArrayXXcd& out) {
        resize(in.rows(), in.cols(), sign);
        out.resize(nRows, nCols);
        transform(in.data(), out.data(), nRows, nCols, sign);
    }

    void FourierTransform::compute(const Eigen::ArrayXcd& in, Eigen::ArrayXcd& out) {
        resize(in.rows(), in.cols(), sign);
        out.resize(nRows, nCols);
        transform(in.data(), out.data(), nRows, nCols, sign);
    }

    void FourierTransform::setSign(int sign) {
        if (sign != FORWARD && sign != BACKWARD) {
            throw Exception("The sign of a FourierTransform must be FORWARD or BACKWARD");
        }
        this->sign = sign;
    }

}