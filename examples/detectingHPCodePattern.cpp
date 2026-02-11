#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;

/** This example shows how to estimate the pose of HP code markers in an image.
 * 
 * The method is described in the following paper: 
 * 
 * [1] A. N. Andre et al., “Automating Robotic Micro-Assembly of Fluidic Chips 
 * and Single Fiber Compression Tests Based-on ΘVisual Measurement With 
 * High-Precision Fiducial Markers, IEEE Transactions on Automation Science and 
 * Engineering, 2022.
 */
int main() {

    // Loading the image
    string filename = "HPCodeImage_20um.png";
    Mat image = cv::imread(filename);

    // Detecting the pattern and estimating its pose
    double physicalPeriod = 20; // µm
    int snapshotSize = 256; // pixels
    HPCodePatternDetector detector(physicalPeriod, 33, snapshotSize);
    detector.compute(image);

    // Displaying the pose if a pattern has been found
    cout << "Detector: " << detector.toString() << endl;
    if (detector.patternFound()) {
        cout << "Found " << detector.markers.size() << " markers in " << filename << endl;
        for (auto it : detector.markers) {
            cout << "  Stamp " << it.first << " at " << it.second.toString() << endl;
        }
        detector.draw(image);
    } else {
        cout << "Marker not found..." << endl;
    }

    // Showing image and its spectrum
    detector.showControlImages();
    imshow(filename, image);
    waitKey(3000);
}
