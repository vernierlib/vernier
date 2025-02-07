#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a pattern from a virtual camera at a 2D pose 
 * and with an orthographic projection
 */
int main() {
    
    // Loading the layout
    string filename = "megarenaPattern.json";
    //PatternLayout* layout = Layout::loadFromJSON(filename);
    PatternLayout* layout = new MegarenaPatternLayout(15.0, 12);
//    PatternLayout* layout = new PeriodicPatternLayout(15.0, 121,251);
    // Setting the pose for rendering
    double x = -15*100;//-6000.0;
    double y = -15*81;//-3000.0;
    double alpha = 0.2;
    double pixelSize = 1.5;
    Pose patternPose = Pose(x, y, alpha, pixelSize);
    
    // Rendering
    ArrayXXd array(512, 512);
    layout->renderOrthographicProjection(patternPose, array);

    // Showing the array
    arrayShow(layout->getDescription() + patternPose.toString() + " (" + filename + ")", array);
    
    layout->saveToJSON("test.json");
}
