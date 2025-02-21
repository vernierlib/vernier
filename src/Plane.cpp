/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#include "Plane.hpp"

namespace vernier {

    Plane::Plane() {
    }

    Plane::Plane(Eigen::Vector3d planeCoefficients) {
        this->planeCoefficients = planeCoefficients;
        ASSERT(!planeCoefficients.isZero());
    }

    Eigen::Vector3d Plane::getCoefficients() {
        return planeCoefficients;
    }

    double Plane::getA() {
        return planeCoefficients(0);
    }

    double Plane::getB() {
        return planeCoefficients(1);
    }

    double Plane::getC() {
        return planeCoefficients(2);
    }

    void Plane::setC(double coeffC) {
        planeCoefficients(2) = coeffC;
    }

    double Plane::getPhase(double row, double col) {
        double phiCenter = planeCoefficients.x() * col + planeCoefficients.y() * row + planeCoefficients.z();
        return phiCenter;
    }

    double Plane::getAngle() {
        return atan2(planeCoefficients(1), planeCoefficients(0));
    }

    double Plane::getPosition(double physicalPeriod, double row, double col, int codePosition) {
        double phiCenter = getPhase(row, col);
        if (physicalPeriod != 0) {
            return physicalPeriod * (phiCenter / (2.0 * PI) + (double) abs(codePosition));
        } else {
            return (2.0 * PI / (sqrt(pow(planeCoefficients(0), 2) + pow(planeCoefficients(1), 2)))) * (phiCenter / (2.0 * PI) + (double) abs(codePosition));
        }
    }

    double Plane::getPositionPixels(double row, double col, int codePosition) {
        double phiCenter = getPhase(row, col);
        return (1 / (sqrt(pow(planeCoefficients(0), 2) + pow(planeCoefficients(1), 2)))) * (phiCenter + (double) abs(codePosition));
    }

    void Plane::revertCoefficients() {
        planeCoefficients(0) = -planeCoefficients(0);
        planeCoefficients(1) = -planeCoefficients(1);
        planeCoefficients(2) = -planeCoefficients(2);
    }

    double Plane::getPixelicPeriod() {
        return 2.0 * PI / (sqrt(pow(planeCoefficients(0), 2) + pow(planeCoefficients(1), 2)));
    }

    std::string Plane::toString() {
        std::string planeStr;
        planeStr = "phi = " + vernier::to_string(planeCoefficients(0)) + "x ";
        if (planeCoefficients(1) < 0) {
            planeStr += "- ";
        } else {
            planeStr += "+ ";
        }
        planeStr += vernier::to_string(abs(planeCoefficients(1))) + "y ";
        if (planeCoefficients(2) < 0) {
            planeStr += "- ";
        } else {
            planeStr += "+ ";
        }
        planeStr += vernier::to_string(abs(planeCoefficients(2)));
        return planeStr;
    }
}