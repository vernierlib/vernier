#include "Vernier.hpp"

using namespace Vernier;
using namespace std;

/** This example converts a pattern layout from JSON to SVG.
 */
int main() {

    // Loading the layout
    string filename = "bitmapPattern";
    PatternLayout* layout = Layout::loadFromJSON(filename + ".json");

    // Writing the layout in a SVG file
    layout->saveToSVG(filename + ".svg");

    cout << filename + ".svg generation completed." << endl;
}
