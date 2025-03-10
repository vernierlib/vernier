/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Plane.hpp"

namespace vernier {

    Plane::Plane() : Plane(0.0, 0.0, 0.0) {
    }

    Plane::Plane(Eigen::Vector3d planeCoefficients) {
        a = planeCoefficients(0);
        b = planeCoefficients(1);
        c = planeCoefficients(2);
    }

    Plane::Plane(double a, double b, double c) {
        this-> a = a;
        this ->b = b;
        this->c = c;
    }

    Eigen::Vector3d Plane::getCoefficients() {
        return Eigen::Vector3d(a, b, c);
    }

    double Plane::getA() {
        return a;
    }

    double Plane::getB() {
        return b;
    }

    double Plane::getC() {
        return c;
    }

    void Plane::setA(double a) {
        this->a = a;
    }

    void Plane::setB(double b) {
        this->b = b;
    }

    void Plane::setC(double c) {
        this->c = c;
    }

    double Plane::getPhase(double y, double x) {
        double phiCenter = a * x + b * y + c;
        return phiCenter;
    }

    double Plane::getAngle() {
        return std::atan2(b, a);
    }

    double Plane::getPosition(double physicalPeriod, double y, double x, int periodShift) {
        double phiCenter = getPhase(y, x);
        if (physicalPeriod != 0.0) {
            return physicalPeriod * (phiCenter / (2.0 * PI) + (double) (periodShift));
        } else {
            return (2.0 * PI / (std::sqrt(pow(a, 2) + pow(b, 2)))) * (phiCenter / (2.0 * PI) + (double) (periodShift));
        }
    }

    double Plane::getPositionPixels(double y, double x, int periodShift) {
        double phiCenter = getPhase(y, x);
        return (1.0 / std::sqrt(a * a + b * b)) * (phiCenter + (double) (periodShift));
    }

    void Plane::flip() {
        a = -a;
        b = -b;
        c = -c;
    }

    void Plane::turnClockwise90() {
        std::swap(a, b);
        a = -a;
    }

    void Plane::turnAntiClockwise90() {
        std::swap(a, b);
        b = -b;
    }

    void Plane::turn180() {
        a = -a;
        b = -b;
    }

    double Plane::getPixelicPeriod() {
        return 2.0 * PI / std::sqrt(a * a + b * b);
    }

    std::string Plane::toString() {
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