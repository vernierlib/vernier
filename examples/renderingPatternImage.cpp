#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example shows how to renders an image of a pattern from a virtual camera 
 * at a given pose and with an orthographic projection
 */
int main() {

    string filename = "megarenaPattern.json";

    // Loading the layout
    PatternLayout* layout = Layout::loadFromJSON(filename);
    cout << "Pattern layout: " << layout->toString() << endl;

    // Setting the pose of the pattern in the camera frame
    double x = -6000.0;
    double y = -8000.0;
    double alpha = 0.2;
    double pixelSize = 15.0;
    Pose patternPose = Pose(x, y, alpha, pixelSize);

    // Rendering image
    ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Showing the image
    Mat image = array2image(array);
    imshow(filename, image);
    waitKey();

}
