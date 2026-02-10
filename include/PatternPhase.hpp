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
     * \example analysingImage.cpp
     *    
     */
    class PatternPhase {
    private:
        
        RegressionPlane regressionPlane;
        FourierTransform fft, ifft;
        GaussianFilter gaussianFilter;
        
        Eigen::ArrayXXcd spatial;  // Image of the pattern converted in complex<double> array for FFT computing
        Eigen::ArrayXXcd spectrum, spectrumShifted;
        Eigen::ArrayXXcd spectrumFiltered1;
        Eigen::ArrayXXcd spectrumFiltered2;
        Eigen::Vector3d mainPeak1, mainPeak2;
        Eigen::ArrayXXcd phase1, phase2;
        Eigen::ArrayXXd unwrappedPhase1, unwrappedPhase2;
        
        PhasePlane plane1, plane2;
        
        void compute();
        
        void computeWeakPerspective(int& betaSign, int& gammaSign, double approxPixelPeriod); // a supprimer ?

        
    public:
        
        double MIN_PEAK_POWER = 0.00001;

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
         *	\param image: image of a pattern in an Eigen::ArrayXXd
         */
        void compute(const Eigen::ArrayXXd& image);
          
        /** Computes the phase planes of a given pattern 
         *
         *	\param image: image of a pattern in a cv::Mat
         */
        void compute(const cv::Mat& image);
       
        /** Computes the phase gradients to find the sign of the out-of-plane 
         * angles (works only with slight perspective projection)
         */
        void computePhaseGradients(int& betaSign, int& gammaSign);

        /** Returns true if two peaks with sufficient power have been found */
        bool peaksFound();
        
        /** Displays the images to check the spectrum analyse. */
        void showControlImages();
        
        /** Returns an image of the spectrum with the peak locations */
        cv::Mat getPeaksImage();
        
        cv::Mat getFringesImage();
        
        cv::Mat getImage();

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
        PhasePlane getPlane1();

        /** Returns the second phase plane*/
        PhasePlane getPlane2();
        
        /** Sets the ratio of pixels to crop from the border for the regression */
        void setCropFactor(double cropFactor);

        /** Sets the size of the Gaussian filter */
        void setSigma(double sigma);

        /** Returns the size of the Gaussian filter */
        double getSigma();

        /** Returns the length of the period in pixels */
        double getPixelPeriod();

        int getNRows();

        int getNCols();
    };
}
#endif // PATTERNPHASE_HPP