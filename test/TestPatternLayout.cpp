/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Layout.hpp"
#include "UnitTest.hpp"
#include <fstream>

using namespace vernier;
using namespace std;

void main0() {
    PatternLayout *layout;
    layout = Layout::loadFromJSON("testQRCodePattern.json");
    layout->saveToSVG();
    std::cout << "Genération terminée" << std::endl;
    delete layout;
}

void main1() {
    //Eigen::ArrayXXi bitSequence(1, 1);

    //Eigen::MatioFile file("Megarena/newMask12Bits_x3_2pi.mat", MAT_ACC_RDONLY);
    //file.read_mat("bit_sequence_codee", bitSequence);

    //Eigen::MatioFile file("Megarena/code695mm.mat", MAT_ACC_RDONLY);          
    //file.read_mat("code695mm", bitSequence);

    //std::cout<<bitSequence.cols()<<std::endl;

    //MegarenaPatternLayout layout(1, bitSequence);
    MegarenaPatternLayout layout(0.5, 8);

    layout.regionOfInterest = Rectangle(0, 0, 680, 680);
    layout.setMargins(10);
    layout.unit = "mm";
    layout.saveToSVG();
    //layout.saveToCSV();
    //layout.saveToJSON("testMegarenaPattern2.json");

    std::cout << "Genération terminée" << std::endl;
}

static void main2() {
    PeriodicPatternLayout layout(9, 17, 17);
    layout.saveToSVG();
    layout.saveToCSV();
    layout.saveToJSON("testPeriodicPattern.json");
    std::cout << "Genération terminée" << std::endl;
}

void main3() {
    BitmapPatternLayout layout("femto.png", 9);
    layout.saveToSVG();
    layout.saveToCSV();
    layout.saveToPNG();
    layout.saveToJSON("testBitmapPattern.json");
    std::cout << "Genération terminée" << std::endl;
}

void main4() {
    HPCodePatternLayout layout(10, 37);
    layout.saveToSVG();
    layout.saveToCSV();
    layout.saveToPNG();
    layout.saveToJSON("testQRCodePattern.json");
    std::cout << "Genération terminée" << std::endl;
}

void main5() {
    BitmapPatternLayout layout;
    layout.loadFromJSON("testBitmapPattern.json");
    layout.saveToSVG();
    layout.saveToCSV();
    layout.saveToPNG();
    std::cout << "Genération terminée" << std::endl;
}

void main6() {
    HPCodePatternLayout layout;
    layout.loadFromJSON("testQRCodePattern.json");
    layout.saveToSVG();
    layout.saveToCSV();
    layout.saveToPNG();
    std::cout << "Genération terminée" << std::endl;
}

void main7() {
    CustomPatternLayout layout;
    layout.loadFromCSV("QRCodePatternLayout.csv");
    layout.saveToSVG();
    layout.saveToJSON("testCustomPattern.json");
    std::cout << "Genération terminée" << std::endl;
}

//void main8() {
//    /* This example converts a PNG image file into a finger print pattern layout (JSON).
//     */
//    // Constructing the layout from PNG image file  
//    double period = 21.0;
//    StampMarkerLayout layout;
//    layout.loadFromPNG("femto.png", period);
//    layout.setDescription("Example of finger print pattern");
//
//    // Writing the layout in a JSON file
//    layout.saveToJSON("fingerprintPattern.json");
//
//    std::cout << "bitmapPattern.json generation completed." << std::endl;
//
//    // Setting the pose for rendering
//    double x = 0.0;
//    double y = 0.0;
//    double alpha = 0.2;
//    double beta = 0.3;
//    double gamma = 0.4;
//    double pixelSize = 4.0;
//    Pose patternPose = Pose(x, y, 0, alpha, beta, gamma, pixelSize);
//
//    // Rendering
//    Eigen::ArrayXXd array(512, 512);
//    layout.renderOrthographicProjection(patternPose, array);
//
//    // Showing the array
//    arrayShow(layout.getDescription(), array);
//
//
//}

void runAllTests() {

    START_UNIT_TEST;
    PeriodicPatternLayout layout1(9, 17, 17);
    layout1.saveToJSON();
    layout1.saveToSVG();
    layout1.saveToPNG();
    layout1.loadFromJSON("PeriodicPattern.json");
    layout1.saveToJSON("PeriodicPattern2.json");
    //    remove("PeriodicPattern.json");
    UNIT_TEST(areFilesEqual("PeriodicPattern.json", "PeriodicPattern2.json"));

    START_UNIT_TEST;
    HPCodePatternLayout layout2(10, 37);
    layout2.saveToJSON();
    layout2.saveToSVG();
    layout2.saveToPNG();
    layout2.saveToCSV();
    layout2.loadFromJSON("HPCodePattern.json");
    layout2.saveToJSON("HPCodePattern2.json");
    //    remove("HPCodePattern.json");
    UNIT_TEST(areFilesEqual("HPCodePattern.json", "HPCodePattern2.json"));

    START_UNIT_TEST;
    MegarenaPatternLayout layout3(4.5, 6);
    layout3.saveToJSON();
    layout3.saveToSVG();
    layout3.saveToPNG();
    layout3.loadFromJSON("MegarenaPattern.json");
    layout3.saveToJSON("MegarenaPattern2.json");
    //    remove("MegarenaPattern.json");
    UNIT_TEST(areFilesEqual("MegarenaPattern.json", "MegarenaPattern2.json"));

    START_UNIT_TEST;
    FingerprintPatternLayout layout4("data/vernier.png", 9);
    layout4.saveToJSON("FingerprintPattern.json");
    layout4.saveToSVG();
    layout4.saveToPNG();
    layout4.loadFromJSON("FingerprintPattern.json");
    layout4.saveToJSON("FingerprintPattern2.json");
    //    remove("FingerprintPattern.json");
    UNIT_TEST(areFilesEqual("FingerprintPattern.json", "FingerprintPattern2.json"));

    START_UNIT_TEST;
    BitmapPatternLayout layout5("data/femto.png", 9);
    layout5.saveToJSON("BitmapPattern.json");
    layout5.saveToSVG();
    layout5.saveToPNG();
    layout5.loadFromJSON("BitmapPattern.json");
    layout5.saveToJSON("BitmapPattern2.json");
    //    remove("BitmapPattern.json");
    UNIT_TEST(areFilesEqual("BitmapPattern.json", "BitmapPattern2.json"));

    START_UNIT_TEST;
    CustomPatternLayout layout6;
    layout6.loadFromCSV("HPCodePattern.csv");
    layout6.saveToJSON();
    layout6.loadFromJSON("CustomPattern.json");
    layout6.saveToJSON("CustomPattern2.json");
    //    remove("CustomPattern.json");
    //    remove("HPCodePattern.csv");
    UNIT_TEST(areFilesEqual("CustomPattern.json", "CustomPattern2.json"));

}

int main(int argc, char** argv) {

    //    main4();

    runAllTests();

    return EXIT_SUCCESS;
}