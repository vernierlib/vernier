#include "Vernier.hpp"

using namespace Vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a pattern from a virtual camera at a 3D pose 
 * and with a perspective projection
 */
int main() {

    // Loading the layout
    string filename = "megarenaPattern.json";
    PatternLayout* layout = Layout::loadFromJSON(filename);

    // Setting the pose of the pattern in the camera frame for rendering
    double x = -2 * 15.0;
    double y = -3 * 15.0;
    double z = 100;
    double rz = 0.23;
    double ry = 0.01;
    double rx = 0.02;
    Pose patternPose = Pose(x, y, z, rz, ry, rx);

    // Rendering
    double pixelSize = 1.5;
    double focalLength = 50.0;
    PatternRenderer patternRenderer(layout, pixelSize, focalLength);
    ArrayXXd array(1024, 1024);
    patternRenderer.renderPerspectiveProjection(patternPose, array);

    // Converting the array in opencv mat and showing it
    imshow(layout->getDescription() + " (" + filename + ")", array2image(array));
    waitKey();
}
