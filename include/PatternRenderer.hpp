/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef PATTERNRENDERER_HPP
#define PATTERNRENDERER_HPP

#include "PatternLayout.hpp"

namespace Vernier {

    /** Class to render 2D images of patterns for given poses and projections.
     *
     */
    class PatternRenderer {
    private:

        PatternLayout* patternLayout;

    public:

        /** scale ratio (pixel size in the same unit than the period of the pattern)
         */
        double pixelSize;


        /** focal distance of the pinhole camera model (in the same unit than the period of the pattern)
         */
        double focalLength;

        /** Creates a pattern renderer associated with an existing pattern layout
         *
         *	\param patternLayout: any kind of pattern layout
         *	\param pixelSize: scale ratio (pixel size in the same unit than the period of the pattern)
         *	\param focalLength: focal distance of the pinhole camera model (in the same unit than the period of the pattern)
         */
        PatternRenderer(PatternLayout* patternLayout, double pixelSize = 1.0, double focalLength = 1.0);

        /** Renders an image with a orthographic projection defined by:
         * 
         *    [ u ]   [ 1/pixelSize            0  0  cx ]         [ x ]
         *    [ v ] = [           0  1/pixelSize  0  cy ] * cTp * [ y ]
         *    [ 1 ]   [           0            0  0   1 ]         [ z ]
         *                                                        [ 1 ]
         * 
         *	\param pose: pattern pose in the camera frame
         *	\param outputImage: any size double array 
         *  
         *   cx and cy are calculated to be the image center coordinates.
         */
        void renderOrthographicProjection(Eigen::Matrix4d cTp, Eigen::ArrayXXd & outputImage);
        void renderOrthographicProjection(Pose pose, Eigen::ArrayXXd & outputImage);

        /** Renders an image with a perspective projection defined by the pinhole camera model:
         * 
         *    [ u ]   [ focalLength/pixelSize                      0  cx  0 ]         [ x ]
         *    [ v ] = [                     0  focalLength/pixelSize  cy  0 ] * cTp * [ y ]
         *    [ 1 ]   [                     0                      0   1  0 ]         [ z ]
         *                                                                            [ 1 ]
         * 
         *	\param pose: pattern pose in the camera frame
         *	\param outputImage: any size double array 
         *  
         *   cx and cy are calculated to be the image center coordinates.
         */
        void renderPerspectiveProjection(Eigen::Matrix4d cTp, Eigen::ArrayXXd & outputImage);
        void renderPerspectiveProjection(Pose pose, Eigen::ArrayXXd & outputImage);

        void renderWeakPerspectiveProjection(Pose pose, Eigen::ArrayXXd& outputImage);

    };

}

#endif