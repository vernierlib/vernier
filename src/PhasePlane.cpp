/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "PhasePlane.hpp"

namespace vernier {

    PhasePlane::PhasePlane() : PhasePlane(0.0, 0.0, 0.0) {
    }

    PhasePlane::PhasePlane(Eigen::Vector3d planeCoefficients) {
        a = planeCoefficients(0);
        b = planeCoefficients(1);
        c = planeCoefficients(2);
    }

    PhasePlane::PhasePlane(double a, double b, double c) {
        this-> a = a;
        this ->b = b;
        this->c = c;
    }

    Eigen::Vector3d PhasePlane::getCoefficients() {
        return Eigen::Vector3d(a, b, c);
    }

    double PhasePlane::getA() {
        return a;
    }

    double PhasePlane::getB() {
        return b;
    }

    double PhasePlane::getC() {
        return c;
    }

    void PhasePlane::setA(double a) {
        this->a = a;
    }

    void PhasePlane::setB(double b) {
        this->b = b;
    }

    void PhasePlane::setC(double c) {
        this->c = c;
    }

    double PhasePlane::getPhase(double y, double x) {
        double phiCenter = a * x + b * y + c;
        return phiCenter;
    }

    double PhasePlane::getAngle() {
        return std::atan2(b, a);
    }

    double PhasePlane::getPosition(double physicalPeriod, double y, double x, int periodShift) {
        double phiCenter = getPhase(y, x);
        if (physicalPeriod > 0.0) {
            return physicalPeriod * (phiCenter / (2.0 * PI) + (double) (periodShift));
        } else {
            return (2.0 * PI / (std::sqrt(pow(a, 2) + pow(b, 2)))) * (phiCenter / (2.0 * PI) + (double) (periodShift));
        }
    }

    double PhasePlane::getPositionPixels(double y, double x, int periodShift) {
        double phiCenter = getPhase(y, x);
        return (1.0 / std::sqrt(a * a + b * b)) * (phiCenter + (double) (periodShift));
    }

    void PhasePlane::flip() {
        a = -a;
        b = -b;
        c = -c;
    }

    void PhasePlane::turnClockwise90() {
        std::swap(a, b);
        a = -a;
    }

    void PhasePlane::turnAntiClockwise90() {
        std::swap(a, b);
        b = -b;
    }

    void PhasePlane::turn180() {
        a = -a;
        b = -b;
    }

    double PhasePlane::getPixelicPeriod() {
        return 2.0 * PI / std::sqrt(a * a + b * b);
    }

    std::string PhasePlane::toString() {
        std::string planeStr;
        planeStr = "phi(x,y) = " + to_string(a) + "x ";
        if (b < 0) {
            planeStr += "- ";
        } else {
            planeStr += "+ ";
        }
        planeStr += to_string(std::abs(b)) + "y ";
        if (c < 0) {
            planeStr += "- ";
        } else {
            planeStr += "+ ";
        }
        planeStr += to_string(std::abs(c));
        return planeStr;
    }
}