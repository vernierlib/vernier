/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Vernier.hpp"
#include "UnitTest.hpp"

using namespace vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

void main2d() {
    
            // Constructing the layout
            double physicalPeriod = randomDouble(5.0, 10.0);
            int codeSize = 12;
            PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
            cout << "  Code size: " << codeSize << endl;
            cout << "  Physical period: " << physicalPeriod << endl;

            // Setting the pose of the pattern in the camera frame for rendering
            double x = -300 * 15.0;
            double y = -100 * 15.0;
            double alpha = -0.2;
            double pixelSize = 2.0;
            Pose patternPose = Pose(x, y, alpha, pixelSize);
            cout << "  Pattern pose:   " << patternPose.toString() << endl;

            // Rendering
            Eigen::ArrayXXd array(512, 512);
            layout->renderOrthographicProjection(patternPose, array);

            // Detecting and estimating the pose of the pattern
            PatternDetector* detector;
            detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
            detector->compute(array);
            Pose estimatedPose = detector->get2DPose();

            // Printing results 
            cout << "  Estimated pose: " << estimatedPose.toString() << endl;
            detector->showControlImages();
        }

         void main3d() {
            
            // Constructing the layout
            double physicalPeriod = randomDouble(5.0, 10.0);
            int codeSize = 12;
            PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
            cout << "  Code size: " << codeSize << endl;
            cout << "  Physical period: " << physicalPeriod << endl;

            // Setting the pose of the pattern in the camera frame for rendering
            double x = randomDouble(-layout->getWidth() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
            double y = randomDouble(-layout->getHeight() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
            double alpha = randomDouble(-PI, PI);
            double beta = randomDouble(-0.5, 0.5);
            double gamma = randomDouble(-0.5, 0.5);
            double pixelSize = randomDouble(1.0, 1.10);
            Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);
            cout << "  Pattern pose:     " << patternPose.toString() << endl;

            // Rendering
            Eigen::ArrayXXd array(512, 512);
            layout->renderOrthographicProjection(patternPose, array);
            
            // Detecting and estimating the pose of the pattern
            PatternDetector* detector;
            detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
            detector->compute(array);
            std::vector<Pose> estimatedPoses = detector->getAll3DPoses();
            
            // Printing results 
            cout << "  Estimated pose 0: " << estimatedPoses[0].toString() << endl;
            cout << "  Estimated pose 1: " << estimatedPoses[1].toString() << endl;
            cout << "  Estimated pose 2: " << estimatedPoses[2].toString() << endl;
            cout << "  Estimated pose 4: " << estimatedPoses[3].toString() << endl;
            detector->showControlImages();

        }

         void main3dPerspective() {

            // Constructing the layout
            double physicalPeriod = 15.0;
            int codeSize = 12;
            PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
            cout << "  Code size: " << codeSize << endl;
            cout << "  Physical period: " << physicalPeriod << endl;

            // Setting the pose of the pattern in the camera frame for rendering
            double x = -200 * 15.0;
            double y = -300 * 15.0;
            double z = 5000.0;
            double alpha = -0.2;
            double beta = 0.10;
            double gamma = 0.05;
            double pixelSize = 2.0;
            Pose patternPose = Pose(x, y, z, alpha, beta, gamma, pixelSize);
            cout << "  Pattern pose:     " << patternPose.toString() << endl;

            // Rendering
            Eigen::ArrayXXd array(512, 512);
            layout->renderPerspectiveProjection(patternPose, array, 5000.0);

            // Detecting and estimating the pose of the pattern
            MegarenaPatternDetector* detector;
            detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
            detector->setDouble("sigma", 5);
            detector->setDouble("cropFactor", 0.8);
            detector->setDouble("pixelPeriod", 7);
            detector->setPhaseGradientMode();
            detector->compute(array);
            std::vector<Pose> estimatedPoses = detector->getAll3DPoses();

            // Printing results 
            cout << "  Estimated pose 0: " << estimatedPoses[0].toString() << endl;
            cout << "  Estimated pose 1: " << estimatedPoses[1].toString() << endl;
            cout << "  Estimated pose 2: " << estimatedPoses[2].toString() << endl;
            cout << "  Estimated pose 4: " << estimatedPoses[3].toString() << endl;
            detector->showControlImages();
        }

         void test2d(int codeSize) {
            START_UNIT_TEST;

            // Constructing the layout
            double physicalPeriod = randomDouble(5.0, 10.0);
            PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
            cout << "  Code size: " << codeSize << endl;
            cout << "  Physical period: " << physicalPeriod << endl;

            // Setting the pose of the pattern in the camera frame for rendering
            double x = randomDouble(-layout->getWidth() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
            double y = randomDouble(-layout->getHeight() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
            double alpha = randomDouble(-PI, PI);
            double pixelSize = randomDouble(1.0, 1.1);
            Pose patternPose = Pose(x, y, alpha, pixelSize);
            cout << "  Pattern pose:   " << patternPose.toString() << endl;

            // Rendering
            Eigen::ArrayXXd array(512, 512);
            layout->renderOrthographicProjection(patternPose, array);

            // Detecting and estimating the pose of the pattern
            PatternDetector* detector;
            detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
            detector->compute(array);
            Pose estimatedPose = detector->get2DPose();

            // Printing results 
            cout << "  Estimated pose: " << estimatedPose.toString() << endl;
//            detector->showControlImages();
//            waitKey(0);
            
            TEST_EQUALITY(patternPose, estimatedPose, 0.01)
        }

         void test3d(int codeSize) {
            START_UNIT_TEST;

            // Constructing the layout
            double physicalPeriod = randomDouble(5.0, 10.0);
            PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);
            cout << "  Code size: " << codeSize << endl;
            cout << "  Physical period: " << physicalPeriod << endl;

            // Setting the pose of the pattern in the camera frame for rendering
            double x = randomDouble(-layout->getWidth() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
            double y = randomDouble(-layout->getHeight() + 3 * codeSize*physicalPeriod, -3 * codeSize * physicalPeriod);
            double alpha = randomDouble(-PI, PI);
            double beta = randomDouble(-0.5, 0.5);
            double gamma = randomDouble(-0.5, 0.5);
            double pixelSize = randomDouble(1.0, 1.10);
            Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);
            cout << "  Pattern pose:     " << patternPose.toString() << endl;

            // Rendering
            Eigen::ArrayXXd array(512, 512);
            layout->renderOrthographicProjection(patternPose, array);
            
            // Detecting and estimating the pose of the pattern
            PatternDetector* detector;
            detector = new MegarenaPatternDetector(physicalPeriod, codeSize);
            detector->compute(array);
            std::vector<Pose> estimatedPoses = detector->getAll3DPoses();
            
            // Printing results 
            cout << "  Estimated pose 0: " << estimatedPoses[0].toString() << endl;
            cout << "  Estimated pose 1: " << estimatedPoses[1].toString() << endl;
            cout << "  Estimated pose 2: " << estimatedPoses[2].toString() << endl;
            cout << "  Estimated pose 4: " << estimatedPoses[3].toString() << endl;
//            detector->showControlImages();
//            waitKey(0);

            UNIT_TEST(areEqual(patternPose, estimatedPoses[0], 0.1)
                    || areEqual(patternPose, estimatedPoses[1], 0.1)
                    || areEqual(patternPose, estimatedPoses[2], 0.1)
                    || areEqual(patternPose, estimatedPoses[3], 0.1))
        }

         void runAllTests() {
            REPEAT_TEST(test2d(8), 10)
            REPEAT_TEST(test2d(10), 10)
            REPEAT_TEST(test2d(12), 10)
            REPEAT_TEST(test3d(8), 10);
        }

         double speed(unsigned long testCount) {

            // Constructing the layout
            double physicalPeriod = 15.0;
            int codeSize = 12;
            PatternLayout* layout = new MegarenaPatternLayout(physicalPeriod, codeSize);

            // Setting the pose of the pattern in the camera frame for rendering
            double x = -300 * 15.0;
            double y = -100 * 15.0;
            double alpha = -0.2;
            double pixelSize = 2.0;
            Pose patternPose = Pose(x, y, alpha, pixelSize);

            // Rendering
            Eigen::ArrayXXd array(1024, 1024);
            layout->renderOrthographicProjection(patternPose, array);

            // Detecting and estimating the pose of the pattern
            PatternDetector* detector;
            detector = new MegarenaPatternDetector(physicalPeriod, 12);
            detector->compute(array);
            Pose estimatedPose = detector->get2DPose();

            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                detector->compute(array);
            }

            return toc(testCount);
        }


int main(int argc, char** argv) {

//    main3d();
    
//    cout << "Computing time: " << speed(10) << " ms" << endl;
    
    runAllTests();

    return EXIT_SUCCESS;
}