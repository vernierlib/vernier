#include "Vernier.hpp"

using namespace Vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example captures an image of a megarena pattern and estimates its 2D pose 
 */
int main() {

    // Initializing VideoCapture
    VideoCapture cap;
    int deviceID = 0; // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    cap.open(deviceID, apiID);
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    
    // Waiting for a new frame from camera
     Mat frame;
     cap.read(frame);
    if (frame.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
        return -1;
    }
    imshow("Camera", frame);

    // Detecting and estimating the pose of the pattern
    PatternDetector* detector;
    detector = Detector::loadFromJSON("megarenaPattern.json");
    detector->compute(frame);

    // Printing results 
    cout << "Estimated pose: " << detector->get2DPose().toString() << endl;

    // Showing image and its spectrum
    detector->showControlImages();
    
    return 0;
}
