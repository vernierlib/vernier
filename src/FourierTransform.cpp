/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "FourierTransform.hpp"

namespace vernier {
#ifdef USE_FFTW

    FourierTransform::FourierTransform(int sign) {
        plan = NULL;
        nRows = 0;
        nCols = 0;
        this -> sign = sign;
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

    FourierTransform::~FourierTransform() {
        if (plan != NULL) {
            fftw_destroy_plan(plan);
        }
    }

    void FourierTransform::resize(int nRows, int nCols, int sign) {
        if (nRows <= 0 || nCols <= 0) {
            throw Exception("Can't resize a FourierTransform with rows<=0 or cols<=0");
        } else if (nRows != this->nRows || nCols != this->nCols || sign != this->sign) {
            if (plan != NULL) {
                fftw_destroy_plan(plan);
            }

            this->nRows = nRows;
            this->nCols = nCols;
            this->sign = sign;

            fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof (fftw_complex) * nRows * nCols);
            fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof (fftw_complex) * nRows * nCols);

            if (nRows == 1 || nCols == 1) {
                plan = fftw_plan_dft_1d(nRows * nCols, in, out, sign, FFTW_MEASURE);
            } else {
                plan = fftw_plan_dft_2d(nCols, nRows, in, out, sign, FFTW_MEASURE);
            }

            fftw_free(in);
            fftw_free(out);
        }
    }

    void FourierTransform::compute(const Eigen::ArrayXXcd& in, Eigen::ArrayXXcd& out) {
        resize(in.rows(), in.cols(), sign);
        out.resize(nRows, nCols);
        fftw_execute_dft(plan, (fftw_complex*) in.data(), (fftw_complex*) out.data());
    }

    void FourierTransform::compute(const Eigen::ArrayXcd& in, Eigen::ArrayXcd& out) {
        resize(in.rows(), in.cols(), sign);
        out.resize(nRows, nCols);
        fftw_execute_dft(plan, (fftw_complex*) in.data(), (fftw_complex*) out.data());
    }
    
    void FourierTransform::setSign(int sign) {
        resize(nRows, nCols, sign);
    }

#else

#include "ooura/fft2d/fftsg.cpp"
#include "ooura/fft2d/fftsg2d.cpp"

    FourierTransform::FourierTransform(int sign) {
        data = NULL;
        workArea = NULL;
        bitReversal = NULL;
        cosSinTable = NULL;
        nRows = 0;
        nCols = 0;
        this -> sign = sign;
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

    FourierTransform::~FourierTransform() {
        if (workArea != NULL) {
            free(data);
            free(workArea);
            free(bitReversal);
            free(cosSinTable);
        }
    }

    void FourierTransform::resize(int nRows, int nCols, int sign) {
        if (nRows <= 0 || nCols <= 0) {
            throw Exception("Can't resize a FourierTransform with rows<=0 or cols<=0");
        } else if (((nRows & (nRows - 1)) != 0) || ((nCols & (nCols - 1)) != 0)) {
            throw Exception("The dimensions of a FourierTransform must be a power of two");
        } else if (nRows != this->nRows || nCols != this->nCols || sign != this->sign) {
            if (workArea != NULL) {
                free(data);
                free(workArea);
                free(bitReversal);
                free(cosSinTable);
            }

            this->nRows = nRows;
            this->nCols = nCols;
            this->sign = sign;

            data = (double**) malloc(sizeof (double*) * nCols);
            workArea = NULL;
            int n = 2 + (int) sqrt(std::max(nRows, nCols));
            bitReversal = (int*) malloc(sizeof (int) * n);
            n = std::max(nRows, nCols) / 2;
            cosSinTable = (double*) malloc(sizeof (double) * n);
            bitReversal[0] = 0;
        }
    }

    void FourierTransform::compute(Eigen::ArrayXXcd& in, Eigen::ArrayXXcd& out) {
        resize(in.rows(), in.cols(), sign);
        out = in;
        // Eigen is column major but Ooura's fft is row major
        for (int j = 0; j < out.cols(); j++) {
            data[j] = (double*) (&out(0, j));
        }
        cdft2d(nCols, 2 * nRows, sign, data, workArea, bitReversal, cosSinTable);
        out = out.conjugate();
    }

    void FourierTransform::compute(Eigen::ArrayXcd& in, Eigen::ArrayXcd& out) {
        throw Exception("Computing of 1D FFT not yet supported by FourierTransform::compute without FFTW");
    }
    
    void FourierTransform::setSign(int sign) {
        resize(nRows, nCols, sign);
    }


#endif
}