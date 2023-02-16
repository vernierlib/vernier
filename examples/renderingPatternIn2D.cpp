#include "Vernier.hpp"

using namespace Vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a pattern from a virtual camera at a 2D pose 
 * and with an orthographic projection
 */
int main() {

    // Loading the layout
    string filename = "megarenaPattern.json";
    PatternLayout* layout = Layout::loadFromJSON(filename);

    // Setting the pose of the pattern in the camera frame for rendering
    double x = -200 * 15.0;
    double y = -300 * 15.0;
    double rz = 0.2;
    Pose patternPose = Pose(x, y, rz);

    // Rendering
    double pixelSize = 1.0;
    PatternRenderer patternRenderer(layout, pixelSize);
    ArrayXXd array(512, 512);
    patternRenderer.renderOrthographicProjection(patternPose, array);

    // Showing the array
    arrayShow(layout->getDescription() + " (" + filename + ")", array);
}
