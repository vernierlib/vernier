#include "Vernier.hpp"

using namespace Vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a periodic pattern and estimates its 3D pose 
 * with an perspective projection
 */
int main() {

    // Loading the layout
    string filename = "megarenaPattern.json";
    PatternLayout* layout = Layout::loadFromJSON(filename);

    // Setting the pose for rendering
    double x = -200.0 * 15;
    double y = -300.0 * 15;
    double z = 5000;
    double alpha = 0.2;
    double beta = -0.3;
    double gamma = 0.4;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, z, alpha, beta, gamma, pixelSize);
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern pose:     " << patternPose.toString() << endl;

    // Rendering
    ArrayXXd array(512, 512);
    double focalLength = 5000.0;
    layout->renderPerspectiveProjection(patternPose, array, focalLength);

    // Estimating the pose of the pattern
    PatternDetector* detector;
    detector = Detector::loadFromJSON(filename);
    detector->setDouble("sigma", 5);
    detector->setDouble("cropFactor", 0.8);
    detector->setDouble("pixelPeriod", 7);
    detector->setPerspectiveMode();
    detector->compute(array);

    // Printing results 
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose: " << detector->get3DPose().toString() << endl;
    cout << "------------------------------------------------------------------" << endl;

    // Showing image and is spectrum
    detector->showControlImages();
}
