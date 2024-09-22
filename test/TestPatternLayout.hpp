/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTPATTERNLAYOUT_HPP
#define TESTPATTERNLAYOUT_HPP

#include "Layout.hpp"
#include "UnitTest.hpp"
#include <fstream>

namespace Vernier {

    class TestPatternLayout {
    public:

        static void main() {
            PatternLayout *layout;
            layout = Layout::loadFromJSON("testQRCodePattern.json");
            layout->saveToSVG();
            std::cout << "Genération terminée" << std::endl;
            delete layout;
        }

        static void main1() {
            Eigen::ArrayXXi bitSequence(1, 1);
            
            //Eigen::MatioFile file("Megarena/newMask12Bits_x3_2pi.mat", MAT_ACC_RDONLY);
            //file.read_mat("bit_sequence_codee", bitSequence);
            
            //Eigen::MatioFile file("Megarena/code695mm.mat", MAT_ACC_RDONLY);          
            //file.read_mat("code695mm", bitSequence);
            
            //std::cout<<bitSequence.cols()<<std::endl;

            //MegarenaPatternLayout layout(1, bitSequence);
            MegarenaPatternLayout layout(0.5,8);

            layout.setRegionOfInterest(Rectangle(0, 0, 680, 680));
            layout.setMargin(10);
            layout.setUnit("mm");
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

#ifdef USE_OPENCV
        static void main3() {
            BitmapPatternLayout layout("femto.png", 9);
            layout.saveToSVG();
            layout.saveToCSV();
            layout.saveToPNG();
            layout.saveToJSON("testBitmapPattern.json");
            std::cout << "Genération terminée" << std::endl;
        }

        static void main4() {
            QRCodePatternLayout layout(10, 37);
            layout.saveToSVG();
            layout.saveToCSV();
            layout.saveToPNG();
            layout.saveToJSON("testQRCodePattern.json");
            std::cout << "Genération terminée" << std::endl;
        }

        static void main5() {
            BitmapPatternLayout layout;
            layout.loadFromJSON("testBitmapPattern.json");
            layout.saveToSVG();
            layout.saveToCSV();
            layout.saveToPNG();
            std::cout << "Genération terminée" << std::endl;
        }

        static void main6() {
            QRCodePatternLayout layout;
            layout.loadFromJSON("testQRCodePattern.json");
            layout.saveToSVG();
            layout.saveToCSV();
            layout.saveToPNG();
            std::cout << "Genération terminée" << std::endl;
        }
#endif // USE_OPENCV

        static void main7() {
            CustomPatternLayout layout;
            layout.loadFromCSV("QRCodePatternLayout.csv");
            layout.saveToSVG();
            layout.saveToJSON("testCustomPattern.json");
            std::cout << "Genération terminée" << std::endl;
        }
        
#ifdef USE_OPENCV
        static void runAllTests() {
            
            PeriodicPatternLayout layout1(9, 17, 17);
            layout1.saveToJSON();
            layout1.loadFromJSON("PeriodicPattern.json");
            remove("PeriodicPattern.json");
            UNIT_TEST(1);

            QRCodePatternLayout layout2(10, 37);
            layout2.saveToJSON();
            layout2.saveToCSV();
            layout2.loadFromJSON("QRCodePattern.json");
            remove("QRCodePattern.json");
            UNIT_TEST(2);

            MegarenaPatternLayout layout3(4.5, 12);
            layout3.saveToJSON();
            layout3.loadFromJSON("MegarenaPattern.json");
            remove("MegarenaPattern.json");
            UNIT_TEST(3);
            
            FingerprintPatternLayout layout4;
            layout4.loadFromPNG("testfiles/femto.png", 9);
            layout4.saveToJSON("FingerprintPattern.json");
            layout4.loadFromJSON("FingerprintPattern.json");
            remove("FingerprintPattern.json");
            UNIT_TEST(4);
            
            BitmapPatternLayout layout5("testfiles/femto.png", 9);
            layout5.saveToJSON("BitmapPattern.json");
            layout5.loadFromJSON("BitmapPattern.json");
            remove("BitmapPattern.json");
            UNIT_TEST(5);
            
            CustomPatternLayout layout6;
            layout6.loadFromCSV("QRCodePattern.csv");
            layout6.saveToJSON();
            layout6.loadFromJSON("CustomPattern.json");
            remove("CustomPattern.json");
            UNIT_TEST(6);
            
        }       
#endif // USE_OPENCV
        
    };
}

#endif