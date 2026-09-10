/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef SPATIAL_HPP
#define SPATIAL_HPP

#include "Common.hpp"

namespace vernier {

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
     *	The algorithm first unwraps the central row (sequentially, from the center
     *	towards both sides), then unwraps every column from that row, upwards and
     *	downwards. Columns being independent from one another, the four quarters are
     *	unwrapped in parallel with OpenMP (when available).
     *
     *	\params wrappedPhase: Eigen matrix of the wrapped phase to be unwrapped
     * 
     */
    void quartersUnwrapPhase(Eigen::ArrayXXd& wrappedPhase);

    Eigen::ArrayXXd hannWindow(int size, int exposure = 1);

    void takeSnapshot(int x, int y, int size, const Eigen::ArrayXXd & array, Eigen::ArrayXXd & snapshot);

}
#endif