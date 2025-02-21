/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#include "Pose.hpp"

namespace vernier {

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
        this->alpha = angleInPiPi(alpha);
        this->beta = angleInPiPi(beta);
        this->gamma = angleInPiPi(gamma);
        this->pixelSize = pixelSize;
        is3D = true;
    }

    std::string Pose::toString() const {
        if (is3D) {
            return "[ x=" + vernier::to_string(x) + "; y=" + vernier::to_string(y) + "; z=" + vernier::to_string(z)
                    + "; alpha=" + vernier::to_string(alpha) + "; beta=" + vernier::to_string(beta) + "; gamma=" + vernier::to_string(gamma)
                    + "; pixelSize=" + vernier::to_string(pixelSize) + " ]";
        } else {
            return "[ x=" + vernier::to_string(x) + "; y=" + vernier::to_string(y) + "; alpha=" + vernier::to_string(alpha)
                    + "; pixelSize=" + vernier::to_string(pixelSize) + " ]";
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

    void Pose::draw(cv::Mat & image, double length, std::string name) {
        if (length < 0.0) {
            length = image.cols / 4;
        }
        Eigen::Matrix4d cTp = getCameraToPatternTransformationMatrix();
        double xImg = cTp(0, 3) / pixelSize + image.cols / 2;
        double yImg = cTp(1, 3) / pixelSize + image.rows / 2;
        cv::line(image, cv::Point(xImg, yImg), cv::Point(xImg + length * cos(alpha), yImg + length * sin(alpha)), cv::Scalar(0, 0, 255));
        cv::line(image, cv::Point(xImg, yImg), cv::Point(xImg + length * cos(alpha + PI / 2), yImg + length * sin(alpha + PI / 2)), cv::Scalar(0, 255, 0));
        cv::line(image, cv::Point(xImg, yImg), cv::Point(xImg - length * cos(alpha), yImg - length * sin(alpha)), cv::Scalar(128, 128, 128));
        cv::line(image, cv::Point(xImg, yImg), cv::Point(xImg - length * cos(alpha + PI / 2), yImg - length * sin(alpha + PI / 2)), cv::Scalar(128, 128, 128));
        cv::putText(image, name, cv::Point(xImg + 5, yImg - 5), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255), 1);

    }

    std::ostream& operator<<(std::ostream& os, const Pose& p) {
        os << p.toString();
        return os;
    }

}