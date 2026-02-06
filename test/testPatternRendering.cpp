/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Layout.hpp"
#include "UnitTest.hpp"
#include <fstream>

using namespace vernier;
using namespace std;

void mainOrthographic2d() {

    // Constructing the layout
    double physicalPeriod = 10.0;
    int codeSize = 12;
    //PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
    PatternLayout* layout = new PeriodicPatternLayout(physicalPeriod, 11,21);
    cout << "  Code size: " << codeSize << endl;
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = 0.0;
    double y = 0.0;
    double alpha = 0.2;
    double pixelSize = 0.5;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose:   " << patternPose.toString() << endl;

    // Rendering image
    cv::Mat image(512, 512, CV_64F);
    layout->renderOrthographicProjection(patternPose, image);
    
    // Showing the image
    imshow("Render", image);
    cv::waitKey();
}

void mainOrthographic3d() {

    // Constructing the layout
    double physicalPeriod = 15.0;
    int codeSize = 12;
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
    cout << "  Code size: " << codeSize << endl;
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = -200 * 15.0;
    double y = -300 * 15.0;
    double z = 0.0;
    double alpha = -0.2;
    double beta = 1.10;
    double gamma = 0.05;
    double pixelSize = 0.5;
    Pose patternPose = Pose(x, y, z, alpha, beta, gamma, pixelSize);
    cout << "  Pattern pose:     " << patternPose.toString() << endl;

    // Rendering image
    cv::Mat image(512, 512, CV_64F);
    layout->renderOrthographicProjection(patternPose, image);
    
    // Showing the image
    imshow("Render", image);
    cv::waitKey();
}

void mainPerspective() {

    // Constructing the layout
    double physicalPeriod = 1.0;
    int codeSize = 12;
    PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
    cout << "  Code size: " << codeSize << endl;
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = 0.0;
    double y = -100.0;
    double z = 300.0;
    double alpha = 0.0;
    double beta = 0.5;
    double gamma = 0.0;
    double pixelSize = 0.002;
    Pose patternPose = Pose(x, y, z, alpha, beta, gamma, pixelSize);
    cout << "  Pattern pose:     " << patternPose.toString() << endl;

    // Rendering image
    cv::Mat image(512, 512, CV_64F);
    //layout->renderOrthographicProjection(patternPose, image);
    
    double focalLength = 6; 
    layout->renderPerspectiveProjection(patternPose, image, focalLength);

    // Showing the image
    imshow("Render", image);
    cv::waitKey();
}




int main(int argc, char** argv) {

    //mainOrthographic2d();
    
    mainPerspective();

    //runAllTests();

    return EXIT_SUCCESS;
}