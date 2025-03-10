#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a periodic pattern and estimates its 3D poses 
 * with an orthographic projection
 */
int main() {

    // Loading the layout
    string filename = "periodicPattern.json";
    PatternLayout* layout = Layout::loadFromJSON(filename);

    // Setting the pose for rendering
    double x = 5.0;
    double y = 3.0;
    double alpha = 0.2;
    double beta = 0.3;
    double gamma = 0.4;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);
    cout << "------------------------------------------------------------------" << endl;
    cout << "Pattern pose:     " << patternPose.toString() << endl;

    // Rendering
    ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Estimating the pose of the pattern
    PatternDetector* detector;
    detector = Detector::loadFromJSON(filename);
    detector->computeArray(array);

    // Printing results 
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose 0: " << detector->getAll3DPoses()[0].toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose 1: " << detector->getAll3DPoses()[1].toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose 2: " << detector->getAll3DPoses()[2].toString() << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Estimated pose 3: " << detector->getAll3DPoses()[3].toString() << endl;
    cout << "------------------------------------------------------------------" << endl;

    // Showing image and is spectrum
    detector->showControlImages();
    cout << "Press <Enter> to exit..." << endl;
    cin.get();
}
