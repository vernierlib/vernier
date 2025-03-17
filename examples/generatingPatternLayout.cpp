#include "Vernier.hpp"

using namespace vernier;
using namespace std;

/** This example shows how to generate pattern layouts from a JSON file.
 */
int main() {
    
    //string filename = "megarenaPattern";
    string filename = "stampPattern";
    //string filename = "bitmapPattern";
    
    // Loading the layout
    unique_ptr<PatternLayout> layout(Layout::loadFromJSON(filename + ".json"));
    cout << "Pattern layout: " << layout->toString() << endl;

    // Writing the layout in a PNG file
    layout->saveToPNG(filename + ".png");
    cout << filename + ".png generation completed." << endl;
    
    // Writing the layout in a SVG file
    layout->saveToSVG(filename + ".svg");
    cout << filename + ".svg generation completed." << endl;
    
    // Writing the layout in a OASIS file
    layout->saveToOASIS(filename + ".oas");
    cout << filename + ".oas generation completed." << endl;
    
}
