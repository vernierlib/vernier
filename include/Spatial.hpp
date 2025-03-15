/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef SPATIAL_HPP
#define SPATIAL_HPP

#include "Common.hpp"

namespace vernier {

    /** \brief Computes the unwrapPIng of a phase map (i.e. in [-PI;PI]) using Eigen matrixes
     *
     * The unwrapPIng is done on the same matrix as the wrapped phase. This way we only
     ate once
     * the memory from the wrapped phase map.
     *
     * Example below show how to use this class.
     */
    class Spatial {
    public:


        /** Unwrap the map phase (in place) according to a method based on a division
         *	of tasks around the four quarters of the phase map. This way the starting point of unwrapPIng
         *	is the center of the phase map.
         *
         *		________________________ direction X
         *		|   (1)	  |   (2)   |
         *		|		  |			|
         *		|---------o---------|
         *		|   (3)   |   (4)   |
         *		|_________|_________|
         *		|
         *		|direction Y
         *
         *	The algorithm unwraps first the upper halp of the phase map (i.e. quarters 1 & 2)
         *	then it unwraps the lower half (i.e. quarters 3 & 4).
         *
         *	\params wrappedPhase: Eigen matrix of the wrapped phase to be unwrapped
         * 
         */
        static void quartersUnwrapPhase(Eigen::ArrayXXd& wrappedPhase);

        template<typename _Scalar, int _Rows, int _Cols>
        static void shift(Eigen::Array<_Scalar, _Rows, _Cols>& array) {
            ASSERT((array.rows() % 2 == 0 && array.cols() % 2 == 0)
                    || (array.rows() == 1 && array.cols() % 2 == 0)
                    || (array.rows() % 2 == 0 && array.cols() == 1));
            for (int col = array.cols() - 1; col >= 0; --col) {
                for (int row = array.rows() - 1; row >= 0; --row) {
                    int a = 1 - 2 * ((row + col) & 1);
                    array(row, col) *= a;
                }
            }
        }

    };

    Eigen::ArrayXXd hannWindow(int size, int exposure = 1);
    
    void takeSnapshot(int x, int y, int size, const Eigen::ArrayXXd & array, Eigen::ArrayXXd & snapshot);

}
#endif