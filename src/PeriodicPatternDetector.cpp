/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "PeriodicPatternDetector.hpp"

namespace vernier {

    PeriodicPatternDetector::PeriodicPatternDetector(double physicalPeriod)
    : PatternDetector() {
        ASSERT_MSG(physicalPeriod > 0.0, "The period must be positive.");
        classname = "PeriodicPattern";
        this->physicalPeriod = physicalPeriod;
    }

    void PeriodicPatternDetector::readJSON(const rapidjson::Value& document) {
        PatternDetector::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            physicalPeriod = document["period"].GetDouble();
        } else {
            throw Exception("The file is not a valid periodic pattern file, the period is missing or has a wrong format.");
        }
    }

    void PeriodicPatternDetector::computeImage() {
        patternPhase.compute(array);
        periodShift1 = 0;
        periodShift2 = 0;
        plane1 = patternPhase.getPlane1();
        plane2 = patternPhase.getPlane2();
    }

    Pose PeriodicPatternDetector::get2DPose(int id) {
        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, periodShift1);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, periodShift2);
        double alpha = plane1.getAngle();
        double pixelSize = physicalPeriod / plane1.getPixelicPeriod();

        return Pose(x, y, alpha, pixelSize);
    }

    Pose PeriodicPatternDetector::get3DPose(int id) {
        int betaSign, gammaSign;
        patternPhase.computePhaseGradients(betaSign, gammaSign);
        Pose pose = getAll3DPoses()[0];
        pose.beta *= betaSign;
        pose.gamma *= gammaSign;
        pose.z = 0.0;
        return pose;
    }

    bool PeriodicPatternDetector::patternFound(int id) {
        return patternPhase.peaksFound();
    }

    int PeriodicPatternDetector::patternCount() {
        return (int) patternPhase.peaksFound();
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
    
    Pose PeriodicPatternDetector::get3DPosePerspective(double focalLength) {
        int betaSign, gammaSign;
        patternPhase.computePhaseGradients(betaSign, gammaSign);
        Pose pose = getAll3DPoses()[0];
        pose.beta *= betaSign;
        pose.gamma *= gammaSign;
        pose.z = pose.pixelSize * focalLength;
        return pose;
    }
    
    void PeriodicPatternDetector::get3DPosePerspective(const cv::Mat & cameraMatrix, const cv::Mat & distortionCoefficients, cv::Mat & rvec, cv::Mat & tvec) {

        // cv::SOLVEPNP_IPPE_SQUARE Method is based on the paper of Toby Collins and Adrien Bartoli. 
        // "Infinitesimal Plane-Based Pose Estimation" ([63]). This method is suitable for marker pose estimation. 
        // It requires 4 coplanar object points defined in the following order:
        //    point 0: [-squareLength / 2, squareLength / 2, 0]
        //    point 1: [ squareLength / 2, squareLength / 2, 0]
        //    point 2: [ squareLength / 2, -squareLength / 2, 0]
        //    point 3: [-squareLength / 2, -squareLength / 2, 0]

        Eigen::Matrix2d A;
        A << plane1.a, plane1.b, plane2.a, plane2.b;
        Eigen::Vector2d B;
        B << -2 * PI - plane1.c - 2 * PI*periodShift1, 2 * PI - plane2.c - 2 * PI*periodShift2;
        Eigen::Vector2d X0 = A.colPivHouseholderQr().solve(B);
        B << 2 * PI - plane1.c - 2 * PI*periodShift1, 2 * PI - plane2.c - 2 * PI*periodShift2;
        Eigen::Vector2d X1 = A.colPivHouseholderQr().solve(B);
        B << 2 * PI - plane1.c - 2 * PI*periodShift1, -2 * PI - plane2.c - 2 * PI*periodShift2;
        Eigen::Vector2d X2 = A.colPivHouseholderQr().solve(B);
        B << -2 * PI - plane1.c - 2 * PI*periodShift1, -2 * PI - plane2.c - 2 * PI*periodShift2;
        Eigen::Vector2d X3 = A.colPivHouseholderQr().solve(B);
        
        float centerX = array.cols()/2;
        float centerY = array.rows()/2;
        std::vector<cv::Point2f> markerCorners(4);
        markerCorners[0] = cv::Point2f(centerX + X0(0), centerY + X0(1));
        markerCorners[1] = cv::Point2f(centerX + X1(0), centerY + X1(1));
        markerCorners[2] = cv::Point2f(centerX + X2(0), centerY + X2(1));
        markerCorners[3] = cv::Point2f(centerX + X3(0), centerY + X3(1));
        
        cv::Mat objPoints(4, 1, CV_32FC3);
        objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-physicalPeriod, physicalPeriod, 0);
        objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(physicalPeriod, physicalPeriod, 0);
        objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(physicalPeriod, -physicalPeriod, 0);
        objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-physicalPeriod, -physicalPeriod, 0);

        cv::solvePnP(objPoints, markerCorners, cameraMatrix, distortionCoefficients, rvec, tvec, false, cv::SOLVEPNP_IPPE_SQUARE);
        
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
        PatternDetector::draw(image);

        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, 0);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, 0);
        double alpha = plane1.getAngle();
        double pixelSize = physicalPeriod / plane1.getPixelicPeriod();

        Pose(x, y, alpha, pixelSize).draw(image);
    }

    Eigen::ArrayXXd PeriodicPatternDetector::getUnwrappedPhase1() {
        return patternPhase.getUnwrappedPhase1();
    }

    Eigen::ArrayXXd PeriodicPatternDetector::getUnwrappedPhase2() {
        return patternPhase.getUnwrappedPhase2();
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
        return PatternDetector::getBool(attribute);
    }

    void PeriodicPatternDetector::setBool(const std::string & attribute, bool value) {
        PatternDetector::setBool(attribute, value);
    }
}