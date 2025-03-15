/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef BITMAPTHUMBNAIL_HPP
#define BITMAPTHUMBNAIL_HPP

#include "PatternPhase.hpp"

namespace vernier {

    /** \brief Uses a given periodic array and its phases planes, computes the thumbnail used as a model-reduction */
    class BitmapThumbnail {
    protected:


    public:

        int THUMBNAIL_ZOOM = 5;
        double DELTA_PHASE = PI / 4;

        cv::Mat thumbnail;
        cv::Mat binaryThumbnail;
        Eigen::ArrayXXd numberWhiteDots;
        Eigen::ArrayXXd cumulWhiteDots;

        /** Constructs an empty bitmap thumbnail */
        BitmapThumbnail();

        /** Constructs a bitmap thumbnail with a givean size
         *
         *	\param size: width and height of the thumbnail in pixels
         */
        BitmapThumbnail(int size);

        void resize(int size);

        void compute(const Eigen::ArrayXXd & array, PhasePlane plane1, PhasePlane plane2);

        int hashCode(int angle);

        void showControlImages();

        int size() {
            return thumbnail.cols;
        }


    };
}

#endif