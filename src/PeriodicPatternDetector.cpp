/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "PeriodicPatternDetector.hpp"

namespace vernier {

    PeriodicPatternDetector::PeriodicPatternDetector(double physicalPeriod)
    : PatternDetector() {
        if (physicalPeriod < 0.0) {
            throw Exception("The period must be positive.");
        }
        classname = "PeriodicPattern";
        this->physicalPeriod = physicalPeriod;
        computePhaseGradient = false;
        periodShift1 = 0;
        periodShift2 = 0;
    }

    void PeriodicPatternDetector::readJSON(rapidjson::Value& document) {
        PatternDetector::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            physicalPeriod = document["period"].GetDouble();
        } else {
            throw Exception("The file is not a valid periodic pattern file, the period is missing or has a wrong format.");
        }
    }

    void PeriodicPatternDetector::resize(int nRows, int nCols) {
        patternPhase.resize(nRows, nCols);
    }

    void PeriodicPatternDetector::computeArray(const Eigen::ArrayXXd& array) {
        patternPhase.compute(array);

        plane1 = patternPhase.getPlane1();
        plane2 = patternPhase.getPlane2();

        if (computePhaseGradient==false) {
            betaSign = 1;
            gammaSign = 1;
        } else {
            patternPhase.computePhaseGradients(betaSign, gammaSign);
        }
    }
        
    Pose PeriodicPatternDetector::get2DPose(int id) {
        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, periodShift1);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, periodShift2);
        double alpha = plane1.getAngle();
        double pixelSize = physicalPeriod / plane1.getPixelicPeriod();

        return Pose(x, y, alpha, pixelSize);
    }
    
    Pose PeriodicPatternDetector::get3DPose(int id) {
        Pose pose = getAll3DPoses()[0];
        pose.beta *= betaSign;
        pose.gamma *= gammaSign;
        return pose;
    }

    bool PeriodicPatternDetector::patternFound(int id) {
        return patternPhase.peaksFound();
    }

    std::vector<Pose> PeriodicPatternDetector::getAll3DPoses(int id) {

        double u1 = plane1.getA() * 2.0 * PI / (pow(plane1.getA(), 2) + pow(plane1.getB(), 2));
        double v1 = plane1.getB() * 2.0 * PI / (pow(plane1.getA(), 2) + pow(plane1.getB(), 2));
        double u2 = plane2.getA() * 2.0 * PI / (pow(plane2.getA(), 2) + pow(plane2.getB(), 2));
        double v2 = plane2.getB() * 2.0 * PI / (pow(plane2.getA(), 2) + pow(plane2.getB(), 2));

        double alpha = plane1.getAngle();

        double b = (pow(u1, 2) + pow(v1, 2)) / pow(physicalPeriod, 2);
        double g = pow(-u2 * sin(alpha) + v2 * cos(alpha), 2) / pow(physicalPeriod, 2);
        double d = pow(u2 * cos(alpha) + v2 * sin(alpha), 2) / pow(physicalPeriod, 2);

        double s_2 = (b + g + d + sqrt(pow(b + g + d, 2) - 4.0 * b * g)) / 2.0;

        double gamma11 = acos(sqrt(g / s_2));
        double gamma12 = -acos(sqrt(g / s_2));
        double gamma21 = acos(sqrt(g / s_2));
        double gamma22 = -acos(sqrt(g / s_2));

        double beta11 = acos(u1 / sqrt(s_2) / physicalPeriod / cos(alpha));
        double beta12 = acos(u1 / sqrt(s_2) / physicalPeriod / cos(alpha));
        double beta21 = -acos(u1 / sqrt(s_2) / physicalPeriod / cos(alpha));
        double beta22 = -acos(u1 / sqrt(s_2) / physicalPeriod / cos(alpha));

        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, periodShift1);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, periodShift2);

        alpha = angleInPiPi(plane2.getAngle() - PI / 2);

        std::vector<Pose> poseVec = {Pose(x, y, 0, alpha, beta11, gamma11, 1.0 / sqrt(s_2)),
            Pose(x, y, 0, alpha, beta11, gamma12, 1.0 / sqrt(s_2)),
            Pose(x, y, 0, alpha, beta21, gamma21, 1.0 / sqrt(s_2)),
            Pose(x, y, 0, alpha, beta22, gamma22, 1.0 / sqrt(s_2))};

        return poseVec;
    }
    
    std::string PeriodicPatternDetector::toString() {
        return PatternDetector::toString() + " period: " + to_string(this->physicalPeriod) + unit;
    }

    PhasePlane PeriodicPatternDetector::getPlane1() {
        return plane1;
    }

    PhasePlane PeriodicPatternDetector::getPlane2() {
        return plane2;
    }

    void PeriodicPatternDetector::showControlImages() {
        cv::imshow("Found peaks (red = dir 1, green = dir 2)", patternPhase.getPeaksImage());
        //cv::moveWindow("Found peaks (red = dir 1, green = dir 2)", patternPhase.getNCols(), 0);
        cv::imshow("Phase fringes (red = dir 1, green = dir 2)", patternPhase.getFringesImage());
        //cv::moveWindow("Phase fringes (red = dir 1, green = dir 2)", 0, 0);
    }
    
    void PeriodicPatternDetector::draw(cv::Mat& image) {
        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, 0);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, 0);
        double alpha = plane1.getAngle();
        double pixelSize = physicalPeriod / plane1.getPixelicPeriod();

        Pose(x, y, alpha, pixelSize).draw(image);        
    }


    void PeriodicPatternDetector::setPhaseGradientMode(bool isPerspective) {
        this->computePhaseGradient = !isPerspective;
    }

    bool PeriodicPatternDetector::isPhaseGradientMode() {
        return computePhaseGradient;
    }

    Eigen::ArrayXXd PeriodicPatternDetector::getUnwrappedPhase1() {
        return patternPhase.getUnwrappedPhase1();
    }

    Eigen::ArrayXXd PeriodicPatternDetector::getUnwrappedPhase2() {
        return patternPhase.getUnwrappedPhase2();
    }

    void PeriodicPatternDetector::setPixelPeriod(double pixelPeriod) {
        this->patternPhase.setPixelPeriod(pixelPeriod);
    }

    void PeriodicPatternDetector::setSigma(double sigma) {
        this->patternPhase.setSigma(sigma);
    }

    void PeriodicPatternDetector::setPhysicalPeriod(double physicalPeriod) {
        this->physicalPeriod = physicalPeriod;
    }

    void PeriodicPatternDetector::setCropFactor(double cropFactor) {
        this->patternPhase.setCropFactor(cropFactor);
    }

    void PeriodicPatternDetector::setDouble(const std::string & attribute, double value) {
        if (attribute == "physicalPeriod") {
            setPhysicalPeriod(value);
        } else if (attribute == "sigma") {
            setSigma(value);
        } else if (attribute == "pixelPeriod") {
            setPixelPeriod(value);
        } else if (attribute == "cropFactor") {
            setCropFactor(value);
        } else {
            PatternDetector::setDouble(attribute, value);
        }
    }

    double PeriodicPatternDetector::getDouble(const std::string & attribute) {
        if (attribute == "physicalPeriod" || attribute == "period") {
            return physicalPeriod;
        } else if (attribute == "sigma") {
            return patternPhase.getSigma();
        } else if (attribute == "approxPixelPeriod") {
            return patternPhase.getPixelPeriod();
        } else {
            return PatternDetector::getDouble(attribute);
        }
    }

    void* PeriodicPatternDetector::getObject(const std::string & attribute) {
        if (attribute == "patternPhase") {
            return &patternPhase;
        } else if (attribute == "plane1") {
            return &plane1;
        } else if (attribute == "plane2") {
            return &plane2;
        } else {
            return PatternDetector::getObject(attribute);
        }
    }

    bool PeriodicPatternDetector::getBool(const std::string & attribute) {
        if (attribute == "computePhaseGradient") {
            return computePhaseGradient;
        } else if (attribute == "phaseGradientMode") {
            return computePhaseGradient;
        } else {
            return PatternDetector::getBool(attribute);
        }
    }

    void PeriodicPatternDetector::setBool(const std::string & attribute, bool value) {
        if (attribute == "computePhaseGradient") {
            computePhaseGradient = value;
        } else if (attribute == "phaseGradientMode") {
            computePhaseGradient = value;
        } else {
            PatternDetector::setBool(attribute, value);
        }
    }
}