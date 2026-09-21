/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Layout.hpp"
#include "UnitTest.hpp"
#include <fstream>
#include <opencv2/imgcodecs.hpp>

using namespace vernier;
using namespace std;

void main0() {
    std::unique_ptr<PatternLayout> layout = Layout::loadFromJSON("testQRCodePattern.json");
    layout->saveToSVG();
    std::cout << "Genération terminée" << std::endl;
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
    BitmapPatternLayout layout4("data/vernier37x37.png", 9);
    layout4.saveToJSON("FingerprintPattern.json");
    layout4.saveToSVG();
    layout4.saveToPNG();
    layout4.loadFromJSON("FingerprintPattern.json");
    layout4.saveToJSON("FingerprintPattern2.json");
    //    remove("FingerprintPattern.json");
    UNIT_TEST(areFilesEqual("FingerprintPattern.json", "FingerprintPattern2.json"));

    START_UNIT_TEST;
    BitmapPatternLayout layout5("data/femto117x45.png", 9);
    layout5.saveToJSON("BitmapPattern.json");
    layout5.saveToSVG();
    layout5.saveToPNG();
    layout5.loadFromJSON("BitmapPattern.json");
    layout5.saveToJSON("BitmapPattern2.json");
    //    remove("BitmapPattern.json");
    UNIT_TEST(areFilesEqual("BitmapPattern.json", "BitmapPattern2.json"));

    START_UNIT_TEST;
    BitmapPatternLayout layout52("data/stamp69x69.png", 20);
    layout52.saveToJSON("StampPattern.json");
    layout52.saveToSVG();
    layout52.saveToPNG();
    layout52.loadFromJSON("StampPattern.json");
    layout52.saveToJSON("StampPattern2.json");
    //    remove("BitmapPattern.json");
    UNIT_TEST(areFilesEqual("StampPattern.json", "StampPattern2.json"));

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

void runRoundedPNGTests() {

    START_UNIT_TEST;
    // 9x9 cells (5x5 dots): an isolated dot, an L-shaped group and two diagonal dots
    cv::Mat cells = cv::Mat::zeros(9, 9, CV_8U);
    cells.at<unsigned char>(1, 1) = 255;
    cells.at<unsigned char>(1, 3) = 255;
    cells.at<unsigned char>(1, 4) = 255;
    cells.at<unsigned char>(2, 4) = 255;
    cells.at<unsigned char>(4, 1) = 255;
    cells.at<unsigned char>(5, 2) = 255;
    cv::imwrite("RoundedCells.png", cells);
    BitmapPatternLayout layout("RoundedCells.png", 10);

    // default settings keep one pixel per cell
    layout.saveToPNG("RoundedCellsDefault.png");
    cv::Mat image = cv::imread("RoundedCellsDefault.png", cv::IMREAD_GRAYSCALE);
    UNIT_TEST(image.size() == cells.size() && cv::countNonZero(image != cells) == 0);

    // square dots are a plain upscaling of the cells
    layout.pngCellSize = 10;
    layout.saveToPNG("RoundedCellsSquare.png");
    image = cv::imread("RoundedCellsSquare.png", cv::IMREAD_GRAYSCALE);
    cv::Mat upscaled;
    cv::resize(cells, upscaled, cv::Size(), 10, 10, cv::INTER_NEAREST);
    UNIT_TEST(cv::countNonZero(image != upscaled) == 0);

    START_UNIT_TEST;
    layout.pngCornerRadius = 0.3;
    layout.saveToPNG("RoundedCellsRound.png");
    image = cv::imread("RoundedCellsRound.png", cv::IMREAD_GRAYSCALE);
    auto pixel = [&image](int row, int col) {
        return image.at<unsigned char>(row, col);
    };
    UNIT_TEST(image.rows == 90 && image.cols == 90);
    // isolated dot: rounded corners, full centre
    UNIT_TEST(pixel(15, 15) == 255 && pixel(10, 10) == 0 && pixel(19, 19) == 0);
    // L-shaped group: outer corners rounded, joined edge kept straight, inner corner filled
    UNIT_TEST(pixel(10, 30) == 0 && pixel(10, 49) == 0);
    UNIT_TEST(pixel(10, 39) == 255 && pixel(10, 40) == 255);
    UNIT_TEST(pixel(20, 39) == 255 && pixel(25, 35) == 0);
    // diagonal dots stay separate
    UNIT_TEST(pixel(49, 19) == 0 && pixel(50, 20) == 0 && pixel(49, 20) == 0 && pixel(50, 19) == 0);

    START_UNIT_TEST;
    layout.pngCornerRadius = 0.6;
    bool thrown = false;
    try {
        layout.saveToPNG("RoundedCellsInvalid.png");
    } catch (Exception &) {
        thrown = true;
    }
    UNIT_TEST(thrown);
}

int main(int argc, char** argv) {

    //    main4();

    runAllTests();

    runRoundedPNGTests();

    return EXIT_SUCCESS;
}