/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Pose.hpp"
#include "UnitTest.hpp"

using namespace vernier;
using namespace std;

void main1() {

    vernier::Pose pose(3, 5, 0.2);

    std::cout << "Pattern pose definition: " << std::endl << pose.toString() << std::endl;

    std::cout << "Pattern pose in camera frame: " << std::endl << pose.getCameraToPatternTransformationMatrix() << std::endl;

    std::cout << "Camera pose in pattern frame: " << std::endl << pose.getPatternToCameraTransformationMatrix() << std::endl;
    
    cv::Mat rvec, tvec;
    pose.getOpenCVRepresentation(rvec, tvec);
    std::cout << "OpenCV representation: " << std::endl;
    std::cout << "rvec" << rvec << std::endl;
    std::cout << "tvec" << tvec << std::endl;
}

void runAllTests() {

    START_UNIT_TEST;
    
    Eigen::ArrayXXd a(4, 4);
    Eigen::ArrayXXd b(4, 4);

    Pose pose = Pose(-2.0, -3.0, 5.0, 0.2, -0.3, 0.4);
    a = pose.getCameraToPatternTransformationMatrix();
    // Matlab code: transl(0,0,5)*r2t(rotz(0.2))*r2t(roty(-0.3))*r2t(rotx(0.4))*transl(-2,-3,0)
    b << 0.936293363584199, -0.295773602360636, -0.189400933088512, -0.985265920086491,
            0.189796060978687, 0.879838033304238, -0.435732131461870, -3.019106221870090,
            0.295520206661340, 0.372025551942260, 0.879923176281257, 3.292882930850542,
            0, 0, 0, 1.000000000000000;

    UNIT_TEST(areEqual(a, b));

    a = pose.getPatternToCameraTransformationMatrix();
    // Matlab code: transl(2,3,0)*r2t(rotx(-0.4))*r2t(roty(0.3))*r2t(rotz(-0.2))*transl(0,0,-5)
    b << 0.936293363584199, 0.189796060978687, 0.295520206661340, 0.522398966693302,
            -0.295773602360636, 0.879838033304238, 0.372025551942260, 1.139872240288702,
            -0.189400933088512, -0.435732131461870, 0.879923176281257, -4.399615881406286,
            0, 0, 0, 1.000000000000000;

    UNIT_TEST(areEqual(a, b));
}

int main(int argc, char** argv) {

    main1();
    
    //runAllTests();

    return EXIT_SUCCESS;
}