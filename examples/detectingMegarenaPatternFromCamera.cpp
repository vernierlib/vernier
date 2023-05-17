#include "Vernier.hpp"

using namespace Vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example renders an image of a megarena pattern and estimates its 2D pose 
 */
int main() {

    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0; // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    
    // wait for a new frame from camera and store it into 'frame'
    cap.read(frame);
    // check if we succeeded
    if (frame.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
        return -1;
    }
    // show live and wait for a key with timeout long enough to show images
    imshow("Live", frame);

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
    
    return 0;
}
