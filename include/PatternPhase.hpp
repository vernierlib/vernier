/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef PATTERNPHASE_HPP
#define PATTERNPHASE_HPP

#include "Common.hpp"
#include "Spectrum.hpp"
#include "Spatial.hpp"
#include "FourierTransform.hpp"
#include "RegressionPlane.hpp"
#include "GaussianFilter.hpp"

namespace vernier {

    /** \brief Computes the phase planes of a given pattern
     * 
     * All the memory allocation and possible pre-calculations are done at the 
     * construction of the detector, then the images can be computed without any delays 
     * (but all the computed images must have the same size).
     *    
     */
    class PatternPhase {
    private:
        
        RegressionPlane regressionPlane;
        FourierTransform fft, ifft;
        GaussianFilter gaussianFilter;
        
        double pixelPeriod;
        int peaksSearchMethod;
        
        Eigen::ArrayXXcd spatial;  // Image of the pattern converted in complex<double> array for FFT computing
        Eigen::ArrayXXcd spectrum, spectrumShifted;
        Eigen::ArrayXXcd spectrumFiltered1;
        Eigen::ArrayXXcd spectrumFiltered2;
        Eigen::Vector3d mainPeak1, mainPeak2;
        Eigen::ArrayXXcd phase1, phase2;
        Eigen::ArrayXXd unwrappedPhase1, unwrappedPhase2;
        
        Plane plane1, plane2;

    public:

        /** Default constructor*/
        PatternPhase();

        /** Constructs a full processing phase retrieving from a given array representing a pattern
         *
         *	\param nRows: number of rows of the pattern
         *	\param nCols: number of columns of the pattern
         */
        PatternPhase(int nRows, int nCols);

        /** Prepares the differences dependants classes and call them in memory if the constructor can't be called on its own.
         *
         *	\param nRows: number of rows of the pattern
         *	\param nCols: number of columns of the pattern
         */
        void resize(int nRows, int nCols);

        /** Computes the phase planes of a given pattern 
         *
         *	\param image: image of a pattern in an ArrayXXd form
         */
        void compute(Eigen::ArrayXXd& image);
        
        /** Computes the phase planes of a given pattern 
         *
         *	\param image: image of a pattern in an ArrayXXcd form
         */
        void compute(Eigen::ArrayXXcd& image);
        
        /** Computes the phase gradients to find the sign of the out-of-plane 
         * angles (works only with perspective projection)
         */
        void computePhaseGradients(int& betaSign, int& gammaSign);

#ifdef USE_OPENCV
        void computeWeakPerspective(Eigen::ArrayXXd& patternArray, int& betaSign, int& gammaSign, double approxPixelPeriod);

        void computeWeakPerspective(Eigen::ArrayXXcd& patternArray, int& betaSign, int& gammaSign, double approxPixelPeriod);
#endif // USE_OPENCV

        /** Computes the phase retrieving with a given QRCode and returns the phase at the center of the pattern along the two directions
         *
         *	\param patternArray: given pattern in an ArrayXXcd form
         */
        void computeQRCode(Eigen::ArrayXXcd& patternArray);

        /** Computes a firstly given image to retrieve all the parameters of the positioning method
         *	Returns the sigma value that best suit the filtering of the spectrum peaks
         *
         *	\param patternArray: given pattern in an ArrayXXcd form
         *	\param pixelPeriod: referenced pixelicPeriod that will be returned
         */
        double computeFirst(Eigen::ArrayXXcd& patternArray, double& pixelPeriod);

#ifdef USE_OPENCV
        /** Returns an image of the spectrum with the peak locations */
        cv::Mat getPeaksImage();
        
        cv::Mat getFringesImage();
        
        cv::Mat getImage();
#endif // USE_OPENCV

        /** Returns the shifted spectrum */
        Eigen::ArrayXXcd & getSpectrum();

        /** Returns the filtered spectrum around peak 1 */
        Eigen::ArrayXXcd & getSpectrumPeak1();

        /** Returns the filtered spectrum around peak 2 */
        Eigen::ArrayXXcd & getSpectrumPeak2();

        /** Returns the first unwrapped phase */
        Eigen::ArrayXXd & getUnwrappedPhase1();

        /** Returns the second unwrapped phase */
        Eigen::ArrayXXd & getUnwrappedPhase2();
        
        /** Returns the first raw (wrapped) phase*/
        Eigen::ArrayXXd  getPhase1();

        /** Returns the second raw (wrapped) phase*/
        Eigen::ArrayXXd  getPhase2();

        /** Returns the first phase plane*/
        Plane getPlane1();

        /** Returns the second phase plane*/
        Plane getPlane2();
        
        /** Sets the ratio of pixels to crop from the border for the regression */
        void setCropFactor(double cropFactor);

        /** Selects the method to find the peaks in the spectrum */     
        void setPeaksSearchMethod(int methodNumber);

        /** Returns the method to find the peaks in the spectrum */
        int getPeaksSearchMethod();

        /** Sets the size of the Gaussian filter */
        void setSigma(double sigma);

        /** Returns the size of the Gaussian filter */
        double getSigma();

        /** Sets the length of period in pixel.
         * 
         * This value is used to search the peaks more effiently if the search method is 2. 
         */
        void setPixelPeriod(double periodLengthInPixel);

        /** Returns the length of the period in pixels */
        double getPixelPeriod();

        int getNRows();

        int getNCols();
    };
}
#endif // PATTERNPHASE_HPP