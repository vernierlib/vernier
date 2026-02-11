#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example shows how to renders an image of a pattern from a virtual camera 
 * at a given pose and with an orthographic projection.
 */
int main() {

    string filename = "megarenaPattern.json";

    // Loading the layout
    unique_ptr<PatternLayout> layout(Layout::loadFromJSON(filename));
    cout << "Pattern layout: " << layout->toString() << endl;

    // Setting the pose (could be 3D)
    double x = -6000.0; // µm
    double y = -8000.0; // µm
    double alpha = 0.2; // rad
    double pixelSize = 2.0; // µm
    Pose patternPose = Pose(x, y, alpha, pixelSize);

    // Rendering image
    Mat image(512, 512, CV_64F);
    layout->renderOrthographicProjection(patternPose, image);

    // Showing the image
    imshow(filename, image);
    waitKey(3000);

}
