#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;

/** This example shows how to analyse the spectrum of an image and to calculate
 * the phase planes of a periodic pattern if two peaks have been found.
 * 
 * The method is described in the paper: A. N. Andre, P. Sandoz, B. Mauze, 
 * M. Jacquot, and G. J. Laurent, Sensing One Nanometer over Ten Centimeters: 
 * A Micro-Encoded Target for Visual In-Plane Position Measurement, IEEE/ASME 
 * Transactions on Mechatronics, 2020.
 * 
 */
int main() {

    // Loading the image
    Mat image = imread("megarenaPatternImage_12bits_9um.jpg");

    // Analysing the spectrum of the image
    PatternPhase patternPhase;
    patternPhase.compute(image);
    
    // Displaying the phase planes if two peaks have been found
    if (patternPhase.peaksFound()) {
        cout << "Phase plane 1: " << patternPhase.getPlane1().toString() <<endl;
        cout << "Phase plane 2: " << patternPhase.getPlane2().toString() <<endl;
    } else {
        cout << "Peaks not found..." << endl;
    }
    
    // Showing image spectrum and fringes 
    patternPhase.showControlImages();
    waitKey(3000);
    
}
