/*
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef CUDAPHASEENGINE_HPP
#define CUDAPHASEENGINE_HPP

#include <complex>

namespace vernier {

    /** \brief GPU implementation of the PatternPhase pipeline (cuFFT + custom kernels).
     *
     * Everything that is data-parallel in PatternPhase::compute() is executed on the
     * device in double precision (cufftZ2Z), so the numerical domain matches the FFTW
     * CPU path: forward FFT, spectrum shift, magnitude, band-pass and angular cuts,
     * peak search, Gaussian filtering, the two inverse FFTs, the checkerboard shift and
     * the phase argument.
     *
     * The public interface takes raw pointers only (no CUDA nor Eigen types) so that this
     * header can be included from the ordinary C++ translation units compiled by the host
     * compiler. The device state (plans, buffers) lives behind an opaque Impl defined in
     * the .cu file.
     *
     * All buffers are laid out column-major (Eigen default), i.e. element (row, col) is at
     * index row + col * nRows, exactly as FFTW sees the Eigen arrays on the CPU path.
     */
    class CudaPhaseEngine {
    public:

        CudaPhaseEngine();

        ~CudaPhaseEngine();

        CudaPhaseEngine(const CudaPhaseEngine&) = delete;

        CudaPhaseEngine& operator=(const CudaPhaseEngine&) = delete;

        /** (Re)allocates the device buffers and cuFFT plans for a given size. */
        void resize(int nRows, int nCols);

        /** Runs the full phase-retrieval pipeline on the GPU.
         *
         * \param spatial          nRows*nCols complex input (the padded image), column-major
         * \param sigma            Gaussian filter radius
         * \param minPeakPower     minimum normalized power for a peak to be accepted
         * \param minFrequency     band-pass lower bound (pixels)
         * \param maxFrequency     band-pass upper bound (pixels)
         * \param smoothingKernelSize  size of the Gaussian blur used before the peak search
         * \param spectrumShifted  [out] nRows*nCols complex, the shifted spectrum
         * \param phase1,phase2    [out] nRows*nCols complex, the filtered/back-transformed phases
         * \param argPhase1,argPhase2 [out] nRows*nCols real, the wrapped phase (arg), NOT yet unwrapped
         * \param peak1,peak2      [out] double[3] = {col, row, power}
         *
         * The final quarters-unwrapping is intentionally left to the caller (CPU): it is a
         * strictly sequential prefix-scan and its result must live on the host anyway.
         */
        void compute(const std::complex<double>* spatial,
                double sigma, double minPeakPower,
                double minFrequency, double maxFrequency,
                int smoothingKernelSize,
                std::complex<double>* spectrumShifted,
                std::complex<double>* phase1, std::complex<double>* phase2,
                double* argPhase1, double* argPhase2,
                double* peak1, double* peak2);

        /** Returns true if at least one usable CUDA device is present at runtime. */
        static bool available();

    private:

        struct Impl;
        Impl* impl;
        int nRows;
        int nCols;
    };
}

#endif // CUDAPHASEENGINE_HPP
