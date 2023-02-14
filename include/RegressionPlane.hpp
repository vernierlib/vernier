/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef REGRESSIONPLANE_HPP
#define REGRESSIONPLANE_HPP

#include "Common.hpp"
#include "Plane.hpp"

namespace Vernier {

    /** \brief Computes the least squares regression plane of a phase map */
    class RegressionPlane {
    private:
        Eigen::Matrix3d matMean;
        Eigen::ArrayXXd meshCol;
        Eigen::ArrayXXd meshRow;
        Eigen::ArrayXXd phaseCropped;
        int colOffset;
        int rowOffset;
        double cropFactor;

    public:

        /** Default constructor with a crop factor of 0.5 */
        RegressionPlane();

        /** Constructs a the plane regressor
         *
         *      \param cropFactor: ratio of pixels to crop from the border
         */
        RegressionPlane(double cropFactor);

        /** Prepares all the prerequired matrices needed to calculate the regression plane
         *
         *	\param rows: size of the phase map along the rows direction
         *	\param cols: size of the phase map along the columns direction
         */
        void resize(int rows, int cols);

        /**	Computes the least square mean plane of the unwrapped phase map.
         *
         *	\params unwrappedPhase: unwrapped phase map as an Eigen matrix
         *	\params planeCoefficients: coefficients of the resulting plane (also the output of the function)
         *		Note that the plane coefficients are stored as :
         *		ax + by + b = z
         *		In the vector [A]
         *			      [B]
         *			      [C]
         *
         */
        Plane compute(const Eigen::ArrayXXd& unwrappedPhase);

        /** Sets the ratio of pixels to crop from the border*/
        void setCropFactor(double cropFactor);
        
        int getColOffset();
        
        int getRowOffset();

        int getNRows();

        int getNCols();

        int getNRowsCropped();

        int getNColsCropped();
        
        

    };
}

#endif