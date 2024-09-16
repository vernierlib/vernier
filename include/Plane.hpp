/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef PLANE_HPP
#define PLANE_HPP

#include "Common.hpp"

namespace Vernier {

    /** \brief With given plane coefficients, constructs a class which can return useful informations about this plane
     *	such as the position or the angle.
     */
    class Plane {
    private:
        Eigen::Vector3d planeCoefficients;

    public:

        Plane();

        /** Constructs a plane with the coefficients of this one
         *	The coefficients have to be stored according to the following equation:
         *	a*x + b*y + c = z
         *
         *	\param planeCoefficients: coefficients of the plane
         */
        Plane(Eigen::Vector3d planeCoefficients);

        /** Returns the coefficients of the plane
         */
        Eigen::Vector3d getCoefficients();

        /** Returns the first coefficient of the plane*/
        double getA();

        /** Returns the second coefficient of the plane*/
        double getB();

        /** Returns the third coefficient of the plane*/
        double getC();

        /** Permits to set the third coefficient of the plane (to set the offset of the plane)
         *
         *	\param coeffC: coefficient to replace in the plane equation
         */
        void setC(double coeffC);

        /** Get the phase at the center of the pattern
         *
         *	\param row: row to compute the phase acquisition
         *	\param col: column to compute the phase acquisition
         */
        double getPhase(double row, double col);

        /** Returns the angle of the plane in radians
         */
        double getAngle();

        /** Returns the position of the pattern in the same unit that the period length. It can return either the relative or the absolute position
         *
         *	\param physicalPeriod: physical period of the pattern
         *	\param row: row where the position is computed
         *	\param col: column where the position is computed
         *	\param codePosition: (optionnal) position of the pattern in the code
         */
        double getPosition(double physicalPeriod, double row = 0.0, double col = 0.0, int codePosition = 0);

        /** Returns the position of the pattern in pixels. It can return either the relative or the absolute position
         *
         *	\param row: row where the position is computed
         *	\param col: column where the position is computed
         *	\param codePosition: (optionnal) position of the pattern in the code
         */
        double getPositionPixels(double row = 0.0, double col = 0.0, int codePosition = 0);

        /** Take the three coefficients and apply a negative sens
         *	i.e. take a*x + b*y + c = z
         *	and returns -a*x -b*y -c = z
         */
        void revertCoefficients();

        /** Returns the pixelic period of the pattern*/
        double getPixelicPeriod();

        std::string toString();
    };

}
#endif