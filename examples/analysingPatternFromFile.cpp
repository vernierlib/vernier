#include "Vernier.hpp"

using namespace vernier;
using namespace cv;
using namespace std;
using namespace Eigen;

/** This example analyse the spectrum of a pattern. 
 */
int main() {

    // Loading the image
    Mat image = cv::imread("megarena.png");
    
    // Analysing the spectrum
    ArrayXXd array = image2array(image);
    PatternPhase patternPhase;
    patternPhase.compute(array);
    
    // Showing image and its spectrum
    imshow("Found peaks (red = dir 1, green = dir 2)", patternPhase.getPeaksImage());
    imshow("Phase fringes (red = dir 1, green = dir 2)", patternPhase.getFringesImage());
    waitKey();
}
