#include "Vernier.hpp"

using namespace Vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a megarena pattern and estimates its 2D pose 
 */
int main() {

    // Loading the layout
    string filename = "megarenaPattern.json";
    PatternLayout* layout = Layout::loadFromJSON(filename);

    // Setting the pose for rendering
    double x = -15 * 400.0;
    double y = -15 * 200.0;
    double alpha = -0.2;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern pose:   " << patternPose.toString() << endl;

    // Rendering
    ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = Detector::loadFromJSON(filename);
    detector->setDouble("pixelPeriod", 7);
    detector->compute(array);

    // Printing results 
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose: " << detector->get2DPose().toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Camera-to-pattern transformation matrix:" << endl << detector->get2DPose().getCameraToPatternTransformationMatrix() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern-to-camera transformation matrix:" << endl << detector->get2DPose().getPatternToCameraTransformationMatrix() << endl;
    cout << "------------------------------------------------------------------" << endl;

    // Showing image and its spectrum
    detector->showControlImages();
}
