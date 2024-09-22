/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef POSE_HPP
#define POSE_HPP

#include "Common.hpp"

namespace Vernier {

    /** This class defines a pose of a pattern from three translations x, y, z and intrinsic 
     * rotations alpha, beta, gamma as defined in [Andre2022pose].
     */
    class Pose {
    public:

        /** Translations (in the same unit than the period of the pattern)
         */
        double x, y, z;

        /** Rotation angle about Z axis (in radian)
         */
        double alpha;

        /** Rotation angle about Y axis (in radian)
         */
        double beta;

        /** Rotation angle about X axis (in radian)
         */
        double gamma;

        /** pixel-to-millimeter scale factor 
         */
        double pixelSize;

        /** If true the pose is defined by x, y, z, alpha, beta, gamma 
         *  else the pose is only defined by x, y, alpha
         */
        bool is3D;

        /** Creates a default null pose
         */
        Pose();

        /** Creates a 2D pose from x, y, alpha
         *
         *	\param x: translation along X axis (in the same unit than the period of the pattern)
         *	\param y: translation along Y axis (in the same unit than the period of the pattern)
         *	\param alpha: rotation angle about the Z axis (in radian)
         *      \param pixelSize: pixel-to-millimeter scale factor
         */
        Pose(double x, double y, double alpha, double pixelSize = 1.0);

        /** Creates a 3D pose from x, y, z, alpha, beta, gamma
         *
         *	\param x: translation along X axis (in the same unit than the period of the pattern)
         *	\param y: translation along Y axis (in the same unit than the period of the pattern)
         *	\param z: translation along Z axis (in the same unit than the period of the pattern)
         *	\param alpha: rotation angle about the Z axis (in radian)
         *	\param beta: rotation angle about the Y intrinsic axis (in radian)
         *	\param gamma: rotation angle about the X intrinnsic axis (in radian)
         *      \param pixelSize: pixel-to-millimeter scale factor
         */
        Pose(double x, double y, double z, double alpha, double beta, double gamma, double pixelSize = 1.0);

        /** Returns a string describing the pose
         */
        std::string toString() const;

        /** Returns the transformation matrix from the camera to the pattern frames defined by:
         * 
         *   cTp = transl(0,0,z).rotz(alpha).roty(beta).rotx(gamma).transl(x,y,0) if the pose is 3D
         * 
         * and by: 
         * 
         *   cTp = rotz(alpha).transl(x,y,0) else
         * 
         */
        Eigen::Matrix4d getCameraToPatternTransformationMatrix();

        /** Returns the transformation matrix between the pattern and the camera frames defined by:
         * 
         *   pTc = transl(-x,-y,0).rotx(-gamma).roty(-beta).rotz(-alpha).transl(0,0,-z) if the pose is 3D
         * 
         * and by: 
         * 
         *   pTc = transl(-x,-y,0).rotz(-alpha) else
         * 
         */
        Eigen::Matrix4d getPatternToCameraTransformationMatrix();

        void draw(cv::Mat & image, double length = -1.0, std::string name ="");
        
    };

    std::ostream& operator<<(std::ostream& os, const Pose& p);    
    
}

#endif