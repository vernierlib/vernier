#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;

/** This example shows how to estimate the 3D pose of a megarena pattern in an image.
 * 
 * The method is described in the following paper: 
 * 
 * [1] A. N. André, P. Sandoz, M. Jacquot, and G. J. Laurent, Pose Measurement 
 * at Small Scale by Spectral Analysis of Periodic Patterns, International Journal 
 * of Computer Vision, 2022.
 */
int main() {
    
    // Loading the image
    string filename = "megarenaPatternImage_8bits_140um.png";
    Mat image = cv::imread(filename);

    // Detecting the pattern and estimating its pose
    double physicalPeriod = 140; // µm
    int codeSize = 8; // bits
    MegarenaPatternDetector detector(physicalPeriod, codeSize);
    detector.compute(image);
     
    // Displaying the pose if a pattern has been found
    cout << "Detector: " << detector.toString() << endl;
    if (detector.patternFound()) {    
        cout << "Estimated 3D pose: " << detector.get3DPose().toString() << endl;
        detector.draw(image);
    } else {
        cout <<"Pattern not found..." << endl;
    }
    
    // Showing image and its spectrum
    detector.showControlImages();
    imshow(filename, image);
    waitKey();
}
