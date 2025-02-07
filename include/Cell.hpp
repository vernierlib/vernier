/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef CELL_HPP
#define CELL_HPP

#include "Common.hpp"
#include "Plane.hpp"

namespace vernier {

    /** \brief Class to use the global cell of a pattern. The global cell represents the mean repartition of dots of the pattern in a cell **/
    class Cell {
    private:
        Eigen::Array33d globalCell;
        Eigen::VectorXd codeOrientation;

    public:
        /** Empty constructor **/
        Cell();

        void resize();

        /** Get the global cell of a pattern. Uses an already computed thumbnail to do this and save computation time
         *
         *	\param thumbnailNumberWhiteDots : thumbnail containing the number of pixels at the center of each cell
         *	\param thumbnailCumulWhiteDots : thumbnail containing the cumul of intensity at the center of each cell
         *
         **/
        void getGlobalCell(Eigen::ArrayXXd thumbnailNumberWhiteDots, Eigen::ArrayXXd thumbnailCumulWhiteDots);

        /** Get the actual code orientation of the pattern modulo 3 by using the super cell
         *
         *	\param superCell : super cell representing the repartition of the white dots as a cell (3x3 dots)
         **/
        Eigen::VectorXd getCodeOrientation();

        /** Simple getter to extract the global cell out of the class
         **/
        Eigen::Array33d getGlobalCellOut();

        /** Display the global cell and its coefficients to monitor the global repartition of the cells
         *
         **/
#ifdef USE_OPENCV
        void guiDisplayCell(cv::Mat& codingCell);
#endif // USE_OPENCV

    };
}
#endif // !CELL_HPP