/*
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "CudaPhaseEngine.hpp"

#include <cuda_runtime.h>
#include <cufft.h>

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Same constant as Utils.hpp so the angular cut matches the CPU path (host + device).
#define PI_D 3.14159265358979323846264338327950288419716939937510

namespace vernier {

    static const int BLOCK = 256;
    static const int RED_BLOCKS = 256; // number of blocks used for the arg-max reduction

#define CUDA_CHECK(call)                                                          \
    do {                                                                          \
        cudaError_t err__ = (call);                                               \
        if (err__ != cudaSuccess) {                                               \
            throw std::runtime_error(std::string("CUDA error: ") +                \
                    cudaGetErrorString(err__) + " at " __FILE__);                 \
        }                                                                         \
    } while (0)

#define CUFFT_CHECK(call)                                                         \
    do {                                                                          \
        cufftResult err__ = (call);                                               \
        if (err__ != CUFFT_SUCCESS) {                                             \
            throw std::runtime_error(std::string("cuFFT error ") +                \
                    std::to_string((int) err__) + " at " __FILE__);               \
        }                                                                         \
    } while (0)

    // ---------------------------------------------------------------------------
    // Device helpers
    // ---------------------------------------------------------------------------

    // Reflect-101 border handling, matching OpenCV's BORDER_REFLECT_101 default.
    __device__ inline int reflect101(int i, int n) {
        if (n == 1) return 0;
        while (i < 0 || i >= n) {
            if (i < 0) i = -i;
            if (i >= n) i = 2 * n - 2 - i;
        }
        return i;
    }

    // Wraps an angle to [-PI, PI), matching Utils.hpp angleInPiPi exactly.
    __device__ inline double angleInPiPi(double angle) {
        while (angle >= PI_D)
            angle -= 2 * PI_D;
        while (angle < -PI_D)
            angle += 2 * PI_D;
        return angle;
    }

    // ---------------------------------------------------------------------------
    // Kernels (column-major indexing: element (row, col) at row + col * nRows)
    // ---------------------------------------------------------------------------

    // Out-of-place fftshift, matching Spectrum.cpp shift(source, dest).
    __global__ void shiftKernel(const cufftDoubleComplex* src, cufftDoubleComplex* dst,
            int nRows, int nCols) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int N = nRows * nCols;
        if (idx >= N) return;
        int r = idx % nRows;
        int c = idx / nRows;
        int nBottom = nRows - nRows / 2;
        int nRight = nCols - nCols / 2;
        int sr = r + nBottom; if (sr >= nRows) sr -= nRows;
        int sc = c + nRight; if (sc >= nCols) sc -= nCols;
        dst[idx] = src[sr + sc * nRows];
    }

    // In-place fftshift by checkerboard sign flip, matching Spectrum.hpp shift(array).
    __global__ void inPlaceShiftKernel(cufftDoubleComplex* a, int nRows, int nCols) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int N = nRows * nCols;
        if (idx >= N) return;
        int r = idx % nRows;
        int c = idx / nRows;
        double s = 1.0 - 2.0 * ((r + c) & 1);
        a[idx].x *= s;
        a[idx].y *= s;
    }

    // Complex magnitude (hypot, matching std::abs(std::complex)).
    __global__ void absKernel(const cufftDoubleComplex* src, double* mag, int N) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= N) return;
        mag[idx] = hypot(src[idx].x, src[idx].y);
    }

    // Band-pass hard cut, matching Spectrum.hpp applyBandPassCut.
    __global__ void bandPassKernel(double* mag, int nRows, int nCols,
            double low, double high) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int N = nRows * nCols;
        if (idx >= N) return;
        int r = idx % nRows;
        int c = idx / nRows;
        int centerX = nRows / 2;
        int centerY = nCols / 2;
        double distance = hypot((double) (r - centerX), (double) (c - centerY));
        if (distance <= low || distance > high)
            mag[idx] = 0.0;
    }

    // Angular sector hard cut, matching Spectrum.hpp applyAngularCut.
    __global__ void angularCutKernel(double* mag, int nRows, int nCols,
            double centerAngle, double widthAngle) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int N = nRows * nCols;
        if (idx >= N) return;
        int r = idx % nRows;
        int c = idx / nRows;
        int centerX = nRows / 2;
        int centerY = nCols / 2;
        double halfWidthAngle = widthAngle / 2.0;
        double currentAngle = atan2((double) (r - centerX), (double) (c - centerY));
        double diff = angleInPiPi(currentAngle - centerAngle);
        if (fabs(diff) <= halfWidthAngle || fabs(diff) >= (PI_D - halfWidthAngle))
            mag[idx] = 0.0;
    }

    // Zeroes the upper half (rows [0, nRows/2)), matching PatternPhase::peaksSearch.
    __global__ void zeroTopHalfKernel(double* mag, int nRows, int nCols) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int N = nRows * nCols;
        if (idx >= N) return;
        int r = idx % nRows;
        if (r < nRows / 2)
            mag[idx] = 0.0;
    }

    // Separable Gaussian blur, row pass (convolve along the row index r).
    __global__ void blurRowsKernel(const double* in, double* out, int nRows, int nCols,
            const double* kern, int ksize) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int N = nRows * nCols;
        if (idx >= N) return;
        int r = idx % nRows;
        int c = idx / nRows;
        int rad = ksize / 2;
        double s = 0.0;
        for (int t = 0; t < ksize; t++) {
            int rr = reflect101(r + (t - rad), nRows);
            s += kern[t] * in[rr + c * nRows];
        }
        out[idx] = s;
    }

    // Separable Gaussian blur, column pass (convolve along the column index c).
    __global__ void blurColsKernel(const double* in, double* out, int nRows, int nCols,
            const double* kern, int ksize) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int N = nRows * nCols;
        if (idx >= N) return;
        int r = idx % nRows;
        int c = idx / nRows;
        int rad = ksize / 2;
        double s = 0.0;
        for (int t = 0; t < ksize; t++) {
            int cc = reflect101(c + (t - rad), nCols);
            s += kern[t] * in[r + cc * nRows];
        }
        out[idx] = s;
    }

    // Hyper-gaussian filter around a peak, matching Spectrum.hpp applyGaussianFilter.
    __global__ void gaussianFilterKernel(cufftDoubleComplex* a, int nRows, int nCols,
            double centerRow, double centerCol, double invTwoSigmaSq) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int N = nRows * nCols;
        if (idx >= N) return;
        int r = idx % nRows;
        int c = idx / nRows;
        double wr = exp(-(r - centerRow) * (r - centerRow) * invTwoSigmaSq);
        double wc = exp(-(c - centerCol) * (c - centerCol) * invTwoSigmaSq);
        double w = wr * wc;
        a[idx].x *= w;
        a[idx].y *= w;
    }

    // Phase argument, matching Eigen .arg() (atan2(imag, real)).
    __global__ void argKernel(const cufftDoubleComplex* src, double* out, int N) {
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= N) return;
        out[idx] = atan2(src[idx].y, src[idx].x);
    }

    // Block-wise arg-max with column-major first-occurrence tie-break, matching
    // Eigen::maxCoeff (keeps the smallest linear index on ties).
    __global__ void argmaxKernel(const double* mag, int N, double* outVal, int* outIdx) {
        __shared__ double sval[BLOCK];
        __shared__ int sidx[BLOCK];
        int tid = threadIdx.x;
        double best = -1e300;
        int bi = INT_MAX;
        for (int j = blockIdx.x * blockDim.x + tid; j < N; j += blockDim.x * gridDim.x) {
            double v = mag[j];
            if (v > best || (v == best && j < bi)) { best = v; bi = j; }
        }
        sval[tid] = best;
        sidx[tid] = bi;
        __syncthreads();
        for (int s = blockDim.x / 2; s > 0; s >>= 1) {
            if (tid < s) {
                double v = sval[tid + s];
                int id = sidx[tid + s];
                if (v > sval[tid] || (v == sval[tid] && id < sidx[tid])) {
                    sval[tid] = v;
                    sidx[tid] = id;
                }
            }
            __syncthreads();
        }
        if (tid == 0) {
            outVal[blockIdx.x] = sval[0];
            outIdx[blockIdx.x] = sidx[0];
        }
    }

    // Final reduction of the per-block partials down to a single (value, index).
    __global__ void finalArgmaxKernel(const double* val, const int* idx, int n,
            double* outVal, int* outIdx) {
        __shared__ double sval[BLOCK];
        __shared__ int sidx[BLOCK];
        int tid = threadIdx.x;
        double best = -1e300;
        int bi = INT_MAX;
        for (int j = tid; j < n; j += blockDim.x) {
            double v = val[j];
            int id = idx[j];
            if (v > best || (v == best && id < bi)) { best = v; bi = id; }
        }
        sval[tid] = best;
        sidx[tid] = bi;
        __syncthreads();
        for (int s = blockDim.x / 2; s > 0; s >>= 1) {
            if (tid < s) {
                double v = sval[tid + s];
                int id = sidx[tid + s];
                if (v > sval[tid] || (v == sval[tid] && id < sidx[tid])) {
                    sval[tid] = v;
                    sidx[tid] = id;
                }
            }
            __syncthreads();
        }
        if (tid == 0) {
            outVal[0] = sval[0];
            outIdx[0] = sidx[0];
        }
    }

    // ---------------------------------------------------------------------------
    // Opaque device state
    // ---------------------------------------------------------------------------

    struct CudaPhaseEngine::Impl {
        cufftHandle plan = 0;
        cufftDoubleComplex *dSpatial = nullptr, *dSpectrum = nullptr, *dShifted = nullptr;
        cufftDoubleComplex *dFilt1 = nullptr, *dFilt2 = nullptr;
        cufftDoubleComplex *dPhase1 = nullptr, *dPhase2 = nullptr;
        double *dMag = nullptr, *dTmp = nullptr, *dArg1 = nullptr, *dArg2 = nullptr;
        double *dKernel = nullptr;
        int kernelCap = 0;
        double *dPartialVal = nullptr;
        int *dPartialIdx = nullptr;
        double *dResVal = nullptr;
        int *dResIdx = nullptr;

        void freeAll() {
            if (plan) { cufftDestroy(plan); plan = 0; }
            cudaFree(dSpatial); cudaFree(dSpectrum); cudaFree(dShifted);
            cudaFree(dFilt1); cudaFree(dFilt2); cudaFree(dPhase1); cudaFree(dPhase2);
            cudaFree(dMag); cudaFree(dTmp); cudaFree(dArg1); cudaFree(dArg2);
            cudaFree(dKernel);
            cudaFree(dPartialVal); cudaFree(dPartialIdx);
            cudaFree(dResVal); cudaFree(dResIdx);
            dSpatial = dSpectrum = dShifted = dFilt1 = dFilt2 = dPhase1 = dPhase2 = nullptr;
            dMag = dTmp = dArg1 = dArg2 = dKernel = dResVal = nullptr;
            dPartialVal = nullptr;
            dPartialIdx = dResIdx = nullptr;
            kernelCap = 0;
        }

        // Reduces dMag to a single (value, linear index) pair on the device, then
        // brings just those two scalars back to the host.
        void argmax(int N, double& outVal, int& outIdx) {
            argmaxKernel<<<RED_BLOCKS, BLOCK>>>(dMag, N, dPartialVal, dPartialIdx);
            finalArgmaxKernel<<<1, BLOCK>>>(dPartialVal, dPartialIdx, RED_BLOCKS,
                    dResVal, dResIdx);
            CUDA_CHECK(cudaMemcpy(&outVal, dResVal, sizeof(double), cudaMemcpyDeviceToHost));
            CUDA_CHECK(cudaMemcpy(&outIdx, dResIdx, sizeof(int), cudaMemcpyDeviceToHost));
        }
    };

    // ---------------------------------------------------------------------------
    // Host side
    // ---------------------------------------------------------------------------

    CudaPhaseEngine::CudaPhaseEngine() : impl(new Impl()), nRows(0), nCols(0) {}

    CudaPhaseEngine::~CudaPhaseEngine() {
        if (impl) {
            impl->freeAll();
            delete impl;
            impl = nullptr;
        }
    }

    bool CudaPhaseEngine::available() {
        int count = 0;
        cudaError_t err = cudaGetDeviceCount(&count);
        return err == cudaSuccess && count > 0;
    }

    void CudaPhaseEngine::resize(int newRows, int newCols) {
        if (newRows <= 0 || newCols <= 0) {
            throw std::runtime_error("CudaPhaseEngine: rows and cols must be positive");
        }
        if (newRows == nRows && newCols == nCols && impl->plan != 0) {
            return;
        }
        impl->freeAll();
        nRows = newRows;
        nCols = newCols;
        size_t N = (size_t) nRows * nCols;
        size_t cbytes = N * sizeof(cufftDoubleComplex);
        size_t dbytes = N * sizeof(double);

        CUDA_CHECK(cudaMalloc(&impl->dSpatial, cbytes));
        CUDA_CHECK(cudaMalloc(&impl->dSpectrum, cbytes));
        CUDA_CHECK(cudaMalloc(&impl->dShifted, cbytes));
        CUDA_CHECK(cudaMalloc(&impl->dFilt1, cbytes));
        CUDA_CHECK(cudaMalloc(&impl->dFilt2, cbytes));
        CUDA_CHECK(cudaMalloc(&impl->dPhase1, cbytes));
        CUDA_CHECK(cudaMalloc(&impl->dPhase2, cbytes));
        CUDA_CHECK(cudaMalloc(&impl->dMag, dbytes));
        CUDA_CHECK(cudaMalloc(&impl->dTmp, dbytes));
        CUDA_CHECK(cudaMalloc(&impl->dArg1, dbytes));
        CUDA_CHECK(cudaMalloc(&impl->dArg2, dbytes));
        CUDA_CHECK(cudaMalloc(&impl->dPartialVal, RED_BLOCKS * sizeof(double)));
        CUDA_CHECK(cudaMalloc(&impl->dPartialIdx, RED_BLOCKS * sizeof(int)));
        CUDA_CHECK(cudaMalloc(&impl->dResVal, sizeof(double)));
        CUDA_CHECK(cudaMalloc(&impl->dResIdx, sizeof(int)));

        // cuFFT plan dims (nCols, nRows) so the DFT indexing matches FFTW's
        // fftw_plan_dft_2d(nCols, nRows, ...) over the same column-major buffer.
        CUFFT_CHECK(cufftPlan2d(&impl->plan, nCols, nRows, CUFFT_Z2Z));
    }

    void CudaPhaseEngine::compute(const std::complex<double>* spatial,
            double sigma, double minPeakPower,
            double minFrequency, double maxFrequency,
            int smoothingKernelSize,
            std::complex<double>* spectrumShifted,
            std::complex<double>* phase1, std::complex<double>* phase2,
            double* argPhase1, double* argPhase2,
            double* peak1, double* peak2) {

        int N = nRows * nCols;
        int grid = (N + BLOCK - 1) / BLOCK;
        size_t cbytes = (size_t) N * sizeof(cufftDoubleComplex);

        // Upload the (complex) padded image and run the forward FFT.
        CUDA_CHECK(cudaMemcpy(impl->dSpatial, spatial, cbytes, cudaMemcpyHostToDevice));
        CUFFT_CHECK(cufftExecZ2Z(impl->plan, impl->dSpatial, impl->dSpectrum, CUFFT_FORWARD));

        // Shift, then take two pristine copies of the shifted spectrum for filtering.
        shiftKernel<<<grid, BLOCK>>>(impl->dSpectrum, impl->dShifted, nRows, nCols);
        CUDA_CHECK(cudaMemcpy(impl->dFilt1, impl->dShifted, cbytes, cudaMemcpyDeviceToDevice));
        CUDA_CHECK(cudaMemcpy(impl->dFilt2, impl->dShifted, cbytes, cudaMemcpyDeviceToDevice));

        // Magnitude, band-pass cut, Gaussian blur, zero the upper half.
        absKernel<<<grid, BLOCK>>>(impl->dShifted, impl->dMag, N);
        bandPassKernel<<<grid, BLOCK>>>(impl->dMag, nRows, nCols, minFrequency, maxFrequency);

        int ksize = smoothingKernelSize;
        if (ksize < 1) ksize = 1;
        {
            // OpenCV getGaussianKernel(ksize, ksize/6.0) coefficients (double).
            double sig = ksize / 6.0;
            double scale2 = -0.5 / (sig * sig);
            std::vector<double> k(ksize);
            double sum = 0.0;
            for (int i = 0; i < ksize; i++) {
                double x = i - (ksize - 1) * 0.5;
                k[i] = std::exp(scale2 * x * x);
                sum += k[i];
            }
            for (int i = 0; i < ksize; i++) k[i] /= sum;
            if (impl->kernelCap < ksize) {
                cudaFree(impl->dKernel);
                CUDA_CHECK(cudaMalloc(&impl->dKernel, ksize * sizeof(double)));
                impl->kernelCap = ksize;
            }
            CUDA_CHECK(cudaMemcpy(impl->dKernel, k.data(), ksize * sizeof(double),
                    cudaMemcpyHostToDevice));
        }
        blurRowsKernel<<<grid, BLOCK>>>(impl->dMag, impl->dTmp, nRows, nCols, impl->dKernel, ksize);
        blurColsKernel<<<grid, BLOCK>>>(impl->dTmp, impl->dMag, nRows, nCols, impl->dKernel, ksize);
        zeroTopHalfKernel<<<grid, BLOCK>>>(impl->dMag, nRows, nCols);

        // Peak search (host control flow, device reductions).
        peak1[0] = peak1[1] = peak1[2] = -1.0;
        peak2[0] = peak2[1] = peak2[2] = -1.0;

        double val;
        int idx;
        impl->argmax(N, val, idx);
        int row = idx % nRows;
        int col = idx / nRows;
        double power = val / nCols / nRows;

        if (power > minPeakPower) {
            peak1[0] = col;
            peak1[1] = row;
            peak1[2] = power;

            double vx = peak1[0] - nCols / 2;
            double vy = peak1[1] - nRows / 2;
            double distance = std::hypot(vx, vy);
            double centerAngle = std::atan2(vy, vx);
            double widthAngle = 2.0 * std::atan2(3.0 * sigma, distance);
            angularCutKernel<<<grid, BLOCK>>>(impl->dMag, nRows, nCols, centerAngle, widthAngle);

            impl->argmax(N, val, idx);
            row = idx % nRows;
            col = idx / nRows;
            power = val / nCols / nRows;
            if (power > minPeakPower) {
                peak2[0] = col;
                peak2[1] = row;
                peak2[2] = power;
                if (peak1[0] < peak2[0]) {
                    for (int i = 0; i < 3; i++) std::swap(peak1[i], peak2[i]);
                }
            }
        }

        // Filter each copy around its peak, inverse-transform, shift and take the arg.
        double invTwoSigmaSq = 1.0 / (2.0 * sigma * sigma);
        gaussianFilterKernel<<<grid, BLOCK>>>(impl->dFilt1, nRows, nCols, peak1[1], peak1[0], invTwoSigmaSq);
        gaussianFilterKernel<<<grid, BLOCK>>>(impl->dFilt2, nRows, nCols, peak2[1], peak2[0], invTwoSigmaSq);

        CUFFT_CHECK(cufftExecZ2Z(impl->plan, impl->dFilt1, impl->dPhase1, CUFFT_INVERSE));
        CUFFT_CHECK(cufftExecZ2Z(impl->plan, impl->dFilt2, impl->dPhase2, CUFFT_INVERSE));

        inPlaceShiftKernel<<<grid, BLOCK>>>(impl->dPhase1, nRows, nCols);
        inPlaceShiftKernel<<<grid, BLOCK>>>(impl->dPhase2, nRows, nCols);

        argKernel<<<grid, BLOCK>>>(impl->dPhase1, impl->dArg1, N);
        argKernel<<<grid, BLOCK>>>(impl->dPhase2, impl->dArg2, N);

        // Bring results back to the host.
        CUDA_CHECK(cudaMemcpy(spectrumShifted, impl->dShifted, cbytes, cudaMemcpyDeviceToHost));
        CUDA_CHECK(cudaMemcpy(phase1, impl->dPhase1, cbytes, cudaMemcpyDeviceToHost));
        CUDA_CHECK(cudaMemcpy(phase2, impl->dPhase2, cbytes, cudaMemcpyDeviceToHost));
        CUDA_CHECK(cudaMemcpy(argPhase1, impl->dArg1, (size_t) N * sizeof(double), cudaMemcpyDeviceToHost));
        CUDA_CHECK(cudaMemcpy(argPhase2, impl->dArg2, (size_t) N * sizeof(double), cudaMemcpyDeviceToHost));

        CUDA_CHECK(cudaGetLastError());
        CUDA_CHECK(cudaDeviceSynchronize());
    }
}
