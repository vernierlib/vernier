/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Vernier.hpp"
#include "UnitTest.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

void main2d() {
    // Constructing the layout
    double physicalPeriod = 15.0;
    PatternLayout* layout = new PeriodicPatternLayout(physicalPeriod, 31, 31);
    cout << "  Physical period: " << physicalPeriod << endl;
    
    // Setting the pose of the pattern in the camera frame for rendering
    double x = 6.0;
    double y = 3.0;
    double alpha = 0.2;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern pose: " << patternPose.toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Camera-to-pattern transformation matrix:" << endl << patternPose.getCameraToPatternTransformationMatrix() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern-to-camera transformation matrix:" << endl << patternPose.getPatternToCameraTransformationMatrix() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Dectecting and estimating the pose of the pattern
    PeriodicPatternDetector* detector;
    detector = new PeriodicPatternDetector(physicalPeriod);
    detector->setDouble("sigma", 1);
    detector->setDouble("cropFactor", 0.4);
    detector->compute(array);

    // Printing results 
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimation of pattern pose: " << detector->get2DPose().toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimation of camera-to-pattern transformation matrix:" << endl << detector->get2DPose().getCameraToPatternTransformationMatrix() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimation of pattern-to-camera transformation matrix:" << endl << detector->get2DPose().getPatternToCameraTransformationMatrix() << endl;

    // Showing image and is spectrum
    PatternPhase *patternPhase = (PatternPhase*) (detector->getObject("patternPhase"));
    arrayShow("Image", array);
    //arrayShow("Phase 1 (wrapped)", patternPhase->getPhase1());
    //arrayShow("Phase 2 (wrapped)", patternPhase->getPhase2());
    //imshow("Spectrum", array2image(patternPhase->getSpectrum()));
    // imshow("spectrumFiltered1", array2image(patternPhase->getSpectrumPeak1()));
    // imshow("spectrumFiltered2", array2image(patternPhase->getSpectrumPeak2()));

    detector->showControlImages();
    //imshow("Spectrum GUI", patternPhase->getControlImage());
    waitKey();
}

void main3dPerspective() {

    // Constructing the layout
    double physicalPeriod = 15;
    PatternLayout* layout = new PeriodicPatternLayout(physicalPeriod, 31, 31);
    cout << "  Physical period: " << physicalPeriod << endl;

    // Setting the pose of the pattern in the camera frame for rendering
    double x = 0 ;
    double y = 0 ;
    double z = 600.0;
    double alpha = 0.0;
    double beta = 0.30;
    double gamma = 0.10;
    double pixelSize = 1.0;
    Pose patternPose = Pose(x, y, z, alpha, beta, gamma);
    cout << "  Pattern pose:     " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    double focalLength = 100/0.5;
    layout->renderPerspectiveProjection(patternPose, array, focalLength);

    // Detecting and estimating the pose of the pattern
    PeriodicPatternDetector* detector;
    detector = new PeriodicPatternDetector(physicalPeriod);
    detector->setDouble("sigma", 15);
    detector->setDouble("cropFactor", 0.8);
    detector->compute(array);
      
    Pose pose = detector->get3DPosePerspective(focalLength);
    cout << "  Estimated pose: " << pose.toString() << endl;
    detector->showControlImages();    
    
    waitKey(0);
}

void test2d() {
    START_UNIT_TEST;
    
    // Constructing the layout
    double physicalPeriod = randomDouble(5.0, 10.0);
    PatternLayout* layout = new PeriodicPatternLayout(physicalPeriod, 81, 61);
    cout << "  Physical period: " << physicalPeriod << endl;
    
    // Setting the pose of the pattern in the camera frame for rendering
    double x = randomDouble(0.0, physicalPeriod / 2.01);
    double y = randomDouble(0.0, physicalPeriod / 2.01);
    double alpha = randomDouble(0, PI / 2);
    double pixelSize = randomDouble(1.0, 1.1);
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "  Pattern pose:   " << patternPose.toString() << endl;

    // Rendering
    Eigen::ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);
    //            arrayShow(layout->getDescription(), array);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = new PeriodicPatternDetector(physicalPeriod);
    detector->compute(array);

    // Printing results 
    Pose estimatedPose;
    if (detector->patternFound()) {
        estimatedPose = detector->get2DPose();
        cout << "  Estimated pose: " << estimatedPose.toString() << endl;
    } else {
        cout << "Pattern not found." << endl;
    }

    TEST_EQUALITY(patternPose, estimatedPose, 0.01)
}

void testBackendSelection() {
    START_UNIT_TEST;

    // A caller holding only a PatternDetector must be able to choose the backend
    // without knowing which detector it is or reaching into its phase engine.
    std::unique_ptr<PatternDetector> detector = Detector::newInstance("PeriodicPattern");

    UNIT_TEST(detector->getBackend() == Backend::CPU);

    // Selecting CPU always works and is visible through the base interface.
    detector->setBackend(Backend::CPU);
    UNIT_TEST(detector->getBackend() == Backend::CPU);

    // The detector's answer must agree with the phase computation it drives.
    //
    // Be aware of what this can and cannot show: with no CUDA device both sides
    // read CPU because that is the default, so this would still hold even if the
    // detector never forwarded anything. Forwarding is only truly demonstrated
    // once a non-default backend is set, which needs a device - see
    // runCudaBackendTests() below. Kept here because it is free and would catch
    // a detector that reported a backend it had never been given.
    PeriodicPatternDetector* periodic = dynamic_cast<PeriodicPatternDetector*> (detector.get());
    UNIT_TEST(periodic != NULL);
    if (periodic == NULL) {
        return;
    }
    UNIT_TEST(periodic->getPatternPhase()->getBackend() == detector->getBackend());

    // The factory must pass its argument on rather than quietly dropping it.
    // Asking for CUDA settles that on any machine: where it is available the
    // detector comes back using it, and where it is not the request is refused.
    // A factory ignoring the argument would do neither - it would hand back a
    // working CPU detector.
    bool factoryUsedTheArgument = false;
    try {
        std::unique_ptr<PatternDetector> viaFactory =
                Detector::newInstance("MegarenaPattern", Backend::CUDA);
        factoryUsedTheArgument = (viaFactory->getBackend() == Backend::CUDA);
    } catch (const std::exception&) {
        factoryUsedTheArgument = true;
    }
    UNIT_TEST(factoryUsedTheArgument);

    // Selecting a backend either takes effect everywhere or changes nothing at
    // all; it must never leave the detector claiming one thing while its phase
    // computation does another. Both outcomes are correct - which one you get
    // depends on the machine - so the property is stated once, for both.
    Backend before = detector->getBackend();
    try {
        detector->setBackend(Backend::CUDA);
        UNIT_TEST(detector->getBackend() == Backend::CUDA);
        UNIT_TEST(periodic->getPatternPhase()->getBackend() == Backend::CUDA);
    } catch (const std::exception&) {
        UNIT_TEST(detector->getBackend() == before);
        UNIT_TEST(periodic->getPatternPhase()->getBackend() == before);
    }
}

void runCudaBackendTests() {
    START_UNIT_TEST;

    if (!cudaAvailable()) {
        std::cout << "No CUDA device found, skipping CUDA tests." << std::endl;
        return;
    }

    // With a device present, a detector switched to CUDA must still measure the
    // same pose as one left on the CPU. Selecting the backend is only useful if
    // it changes how the work is done and not what comes out of it.
    double physicalPeriod = 8.0;
    PeriodicPatternLayout layout(physicalPeriod, 81, 61);
    Pose patternPose = Pose(2.0, 1.5, 0.3, 1.0);
    Eigen::ArrayXXd array(512, 512);
    layout.renderOrthographicProjection(patternPose, array);

    std::unique_ptr<PatternDetector> cpu = Detector::newInstance("PeriodicPattern");
    std::unique_ptr<PatternDetector> cuda =
            Detector::newInstance("PeriodicPattern", Backend::CUDA);

    UNIT_TEST(cuda->getBackend() == Backend::CUDA);

    cpu->compute(array);
    cuda->compute(array);

    UNIT_TEST(cpu->patternFound());
    UNIT_TEST(cuda->patternFound());

    // The choice must survive the computation rather than being reset by it.
    UNIT_TEST(cuda->getBackend() == Backend::CUDA);

    TEST_EQUALITY(cpu->get2DPose(), cuda->get2DPose(), 1e-9)
}

int main(int argc, char** argv) {

    //main2d();

    //main3dPerspective();
    
    testBackendSelection();

    runCudaBackendTests();

    REPEAT_TEST(test2d(), 10)

    return EXIT_SUCCESS;
}