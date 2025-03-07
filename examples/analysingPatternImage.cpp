#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;

/** This example analyse the spectrum of a pattern. 
 */
int main() {

    // Loading the image
    Mat image = cv::imread("megarena.png");

    // Analysing the spectrum
    PatternPhase patternPhase;
    patternPhase.compute(image);

    // Showing image spectrum and fringes if two peaks have been found
    if (patternPhase.peaksFound()) {
        imshow("Found peaks (red = dir 1, green = dir 2)", patternPhase.getPeaksImage());
        imshow("Phase fringes (red = dir 1, green = dir 2)", patternPhase.getFringesImage());
        waitKey();
    } else {
        cout << "Peaks not found..." << endl;
    }
}
