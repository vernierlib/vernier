#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a pattern from a virtual camera at a 3D pose 
 * and with a orthographic projection
 */
int main() {

    // Loading the layout
    string filename = "megarenaPattern.json";
    PatternLayout* layout = Layout::loadFromJSON(filename);

    // Setting the pose for rendering
    double x = -6000.0;
    double y = -3000.0;
    double alpha = 0.2;
    double beta = 0.3;
    double gamma = 0.4;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);

    // Rendering
    ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Showing the array
    arrayShow(layout->getClassname() + " (" + filename + ")", array);
}
