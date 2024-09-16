/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#include "Pose.hpp"

namespace Vernier {

    Pose::Pose()
    : Pose(0.0, 0.0, 0.0, 0.0, 0.0, 0.0) {
    }

    Pose::Pose(double x, double y, double alpha, double pixelSize)
    : Pose(x, y, 0.0, alpha, 0.0, 0.0, pixelSize) {
        is3D = false;
    }

    Pose::Pose(double x, double y, double z, double alpha, double beta, double gamma, double pixelSize) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->alpha = alpha;
        this->beta = beta;
        this->gamma = gamma;
        this->pixelSize = pixelSize;
        is3D = true;
    }

    std::string Pose::toString() {
        if (is3D) {
            return "[ x=" + Vernier::toString(x) + "; y=" + Vernier::toString(y) + "; z=" + Vernier::toString(z)
                    + "; alpha=" + Vernier::toString(alpha) + "; beta=" + Vernier::toString(beta) + "; gamma=" + Vernier::toString(gamma)
                    + "; pixelSize=" + Vernier::toString(pixelSize) + " ]";
        } else {
            return "[ x=" + Vernier::toString(x) + "; y=" + Vernier::toString(y) + "; alpha=" + Vernier::toString(alpha)
                    + "; pixelSize=" + Vernier::toString(pixelSize) + " ]";
        }
    }

    Eigen::Matrix4d Pose::getCameraToPatternTransformationMatrix() {
        Eigen::Matrix4d cTp;
        if (is3D) {
            cTp << cos(alpha) * cos(beta), cos(alpha) * sin(beta) * sin(gamma) - cos(gamma) * sin(alpha), sin(alpha) * sin(gamma) + cos(alpha) * cos(gamma) * sin(beta), x * cos(alpha) * cos(beta) - y * (cos(gamma) * sin(alpha) - cos(alpha) * sin(beta) * sin(gamma)),
                    cos(beta) * sin(alpha), cos(alpha) * cos(gamma) + sin(alpha) * sin(beta) * sin(gamma), cos(gamma) * sin(alpha) * sin(beta) - cos(alpha) * sin(gamma), y * (cos(alpha) * cos(gamma) + sin(alpha) * sin(beta) * sin(gamma)) + x * cos(beta) * sin(alpha),
                    -sin(beta), cos(beta) * sin(gamma), cos(beta) * cos(gamma), z - x * sin(beta) + y * cos(beta) * sin(gamma),
                    0, 0, 0, 1;

        } else {
            cTp << cos(alpha), -sin(alpha), 0, x * cos(alpha) - y * sin(alpha),
                    sin(alpha), cos(alpha), 0, x * sin(alpha) + y * cos(alpha),
                    0, 0, 1, 0,
                    0, 0, 0, 1;
        }
        return cTp;
    }

    Eigen::Matrix4d Pose::getPatternToCameraTransformationMatrix() {
        Eigen::Matrix4d pTc;
        if (is3D) {
            pTc << cos(alpha) * cos(beta), cos(beta) * sin(alpha), -sin(beta), z * sin(beta) - x,
                    cos(alpha) * sin(beta) * sin(gamma) - cos(gamma) * sin(alpha), cos(alpha) * cos(gamma) + sin(alpha) * sin(beta) * sin(gamma), cos(beta) * sin(gamma), -y - z * cos(beta) * sin(gamma),
                    sin(alpha) * sin(gamma) + cos(alpha) * cos(gamma) * sin(beta), cos(gamma) * sin(alpha) * sin(beta) - cos(alpha) * sin(gamma), cos(beta) * cos(gamma), -z * cos(beta) * cos(gamma),
                    0, 0, 0, 1;

        } else {
            pTc << cos(alpha), sin(alpha), 0, -x,
                    -sin(alpha), cos(alpha), 0, -y,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
        }
        return pTc;
    }

}