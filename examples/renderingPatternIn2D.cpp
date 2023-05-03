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

    // Setting the pose for rendering
    double x = -6000.0;
    double y = -3000.0;
    double alpha = -0.2;
    double pixelSize = 2.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    
    // Rendering
    ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Showing the array
    arrayShow(layout->getDescription() + " (" + filename + ")", array);
}
