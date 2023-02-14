#include "Vernier.hpp"

using namespace Vernier;
using namespace std;

/** This example converts a pattern layout from JSON to SVG.
 */
int main() {

    // Loading the layout
    string filename = "megarenaPattern";
    PatternLayout* layout = Layout::loadFromJSON(filename + ".json");

    // Writing the layout in a SVG file
    layout->saveToPNG(filename + ".png");

    cout << filename + ".png generation completed." << endl;
}
