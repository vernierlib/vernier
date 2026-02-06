/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef PHASEPLANE_HPP
#define PHASEPLANE_HPP

#include "Common.hpp"

namespace vernier {

    /** \brief constructs a phase plane with given coefficients. The class returns 
     * useful informations about this plane such as the phase or the angle.
     */
    class PhasePlane {
    public:

        double a, b, c;

        PhasePlane();

        /** Constructs a plane with three coefficients in a vector
         * 
         * The plane is defined according to the following equation: 
         * 
         *      vector[0]*x + vector[1]*y + vector[2] = z
         *
         *	\param vector: coefficients of the plane
         */
        PhasePlane(Eigen::Vector3d planeCoefficients);

        /** Constructs a plane with three coefficients
         *	
         * The plane is defined according to the following equation:
         * 
         *	a*x + b*y + c = z
         *
         *	\param a: first coefficient of the plane
         *      \param b: second coefficient of the plane
         *      \param c: third coefficient of the plane
         */
        PhasePlane(double a, double b, double c);

        /** Returns the coefficients of the plane within a vector
         */
        Eigen::Vector3d getCoefficients();

        /** Returns the first coefficient of the plane*/
        double getA();

        /** Returns the second coefficient of the plane*/
        double getB();

        /** Returns the third coefficient of the plane*/
        double getC();

        /** Sets the first coefficient of the plane
         *
         *	\param a: first coefficient of the plane
         */
        void setA(double a);

        /** Sets the second coefficient of the plane
         *
         *	\param c: second coefficient of the plane
         */
        void setB(double b);

        /** Sets the third coefficient of the plane (offset of the plane)
         *
         *	\param c: third coefficient of the plane
         */
        void setC(double c);

        /** Get the phase at a given position of the image
         *
         *	\param y: row to compute the phase acquisition (default = 0 = center)
         *	\param x: column to compute the phase acquisition (default = 0 = center)
         */
        double getPhase(double y = 0.0, double x = 0.0);

        /** Returns the angle of the plane in radians
         */
        double getAngle();

        /** Returns the position of the pattern in the same unit that the period length. 
         * It can return either the relative or the absolute position
         *
         *	\param physicalPeriod: physical period of the pattern
         *	\param y: row where the position is computed (default = 0 = center)
         *	\param x: column where the position is computed (default = 0 = center)
         *	\param periodShift: number of periods to shift (default = 0)
         */
        double getPosition(double physicalPeriod, double y = 0.0, double x = 0.0, int periodShift = 0);

        /** Returns the position of the pattern in pixels. 
         * It can return either the relative or the absolute position
         *
         *	\param y: row where the position is computed (default = 0 = center)
         *	\param x: column where the position is computed (default = 0 = center)
         *	\param periodShift: number of periods to shift (default = 0)
         */
        double getPositionPixels(double y = 0.0, double x = 0.0, int periodShift = 0);

        /** Replace the plane coefficients by their opposite values.
         * 
         *	from a*x + b*y + c = z
         *	the plane becomes -a*x -b*y -c = z
         */
        void flip();

        void turnClockwise90();

        void turnAntiClockwise90();

        void turn180();

        /** Returns the pixelic period of the pattern*/
        double getPixelicPeriod();

        std::string toString();
    };

}
#endif