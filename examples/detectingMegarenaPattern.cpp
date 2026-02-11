#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;

/** This example shows how to estimate the pose of a megarena pattern in an image.
 * 
 * The method is described in the following papers: 
 * 
 * [1] A. N. Andre, P. Sandoz, B. Mauze, M. Jacquot, and G. J. Laurent, 
 * Sensing One Nanometer over Ten Centimeters: A Micro-Encoded Target for Visual 
 * In-Plane Position Measurement, IEEE/ASME Transactions on Mechatronics, 2020.
 * 
 * [2] A. N. Andre, P. Sandoz, B. Mauze, M. Jacquot, and G. J. Laurent, 
 * Robust phase-based decoding for absolute (X, Y, Θ) positioning by vision, 
 * IEEE Transactions on Instrumentation and Measurement, 2020.
 */
int main() {
    
    // Loading the image
    string filename = "megarenaPatternImage_12bits_9um.jpg";
    Mat image = cv::imread(filename);

    // Detecting the pattern and estimating its pose
    double physicalPeriod = 9; // µm
    int codeSize = 12; // bits
    MegarenaPatternDetector detector(physicalPeriod, codeSize);
    detector.compute(image);
     
    // Displaying the pose if a pattern has been found
    cout << "Detector: " << detector.toString() << endl;
    if (detector.patternFound()) {    
        cout << "Estimated pose: " << detector.get2DPose().toString() << endl;
        detector.draw(image);
    } else {
        cout <<"Pattern not found..." << endl;
    }
    
    // Showing image and its spectrum
    detector.showControlImages();
    imshow(filename, image);
    waitKey(3000);
}
