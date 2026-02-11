#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;

/** This example shows how to estimate the pose of stamp markers in an image.
 */
int main() {

    // Loading the image
    string filename = "stampPatternImage_15um.jpg";
    Mat image = cv::imread(filename);

    // Detecting the pattern and estimating its pose
    double physicalPeriod = 15; // µm
    int snapshotSize = 820; // pixels
    StampPatternDetector detector(physicalPeriod, "stampPattern.png", snapshotSize);
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
