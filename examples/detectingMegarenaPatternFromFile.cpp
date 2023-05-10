#include "Vernier.hpp"

using namespace Vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a megarena pattern and estimates its 2D pose 
 */
int main() {

    // Loading the image
    cv::Mat image = cv::imread("megarena.png");
    
    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = Detector::loadFromJSON("megarenaPattern.json");
    detector->compute(image);

    // Printing results 
    cout << "Estimated pose: " << detector->get2DPose().toString() << endl;
    
    // Showing image and its spectrum
    detector->showControlImages();
}
