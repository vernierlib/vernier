#include "Vernier.hpp"

using namespace Vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a bitmap from a virtual camera at a 3D pose 
 * and with a orthographic projection
 */
int main() {


    // Loading the layout
    double period = 21.0;
    BitmapPatternLayout layout("test.png", period);

    // Setting the pose for rendering
    double x = 0.0;
    double y = 0.0;
    double alpha = 0.2;
    double beta = 0.3;
    double gamma = 0.4;
    double pixelSize = 1.0;
    Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);

    // Rendering
    ArrayXXd array(512, 512);
    layout.renderOrthographicProjection(patternPose, array);

    // Showing the array
    arrayShow(layout.getDescription(), array);
}
