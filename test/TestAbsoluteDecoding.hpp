/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTABSOLUTEDECODING_HPP
#define TESTABSOLUTEDECODING_HPP

#include "Vernier.hpp"
#include "UnitTest.hpp"
#include "AbsoluteDecoding.hpp"
#include <random>
#include "eigen-matio/MatioFile.hpp"

namespace Vernier {

    class TestAbsoluteDecoding {
    public:

        /** Simple main to find the position of a simple coded sequence*/
        static void main() {
            Eigen::ArrayXXi bitSequence(1, 20);
            bitSequence << 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0;

            std::cout << "coded sequence : \n" << bitSequence << std::endl;

            Eigen::ArrayXXd codingSample(1, 8);
            codingSample << 1, 1, 1, 0, 0, 1, 1, 1;

            std::cout << "coding sample : \n" << codingSample << std::endl;

            AbsoluteDecoding absoludeDecode(bitSequence);

            int maxIndex = absoludeDecode.findCodePosition(codingSample, 1);

            std::cout << "max index of correlation at : " << maxIndex << "th position" << std::endl;
        }
        
        static void runAllTests() {
            Eigen::ArrayXXi bitSequence(1, 20);
            bitSequence << 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0;

            //std::cout << "coded sequence : \n" << bitSequence << std::endl;

            Eigen::ArrayXXd codingSample(1, 8);
            codingSample << 1, 1, 1, 0, 0, 1, 1, 1;

           // std::cout << "coding sample : \n" << codingSample << std::endl;

            AbsoluteDecoding absoludeDecode(bitSequence);

            int maxIndex = absoludeDecode.findCodePosition(codingSample, 1);

            //std::cout << "max index of correlation at : " << maxIndex << "th position" << std::endl;
            
            UNIT_TEST(maxIndex==7);
        }
        
        

        static void test12bits() {
            Eigen::ArrayXXi bitSequence(1, 1);
            Eigen::MatioFile file("testfiles/newMask12Bits_x3_2pi.mat", MAT_ACC_RDONLY);
            file.read_mat("bit_sequence_codee", bitSequence);
            file.close();
            std::random_device rd;
            std::uniform_int_distribution<int> dist(200, 10000);
            std::uniform_int_distribution<int> dist2(13, 200);
            AbsoluteDecoding absoludeDecode(bitSequence);

            int codePosition = dist(rd);

            int codeLength = dist2(rd);

            Eigen::ArrayXXd codingSample = bitSequence.block(0, codePosition, 1, codeLength).cast<double>();
            codingSample.transposeInPlace();

            int maxIndex = absoludeDecode.findCodePosition(codingSample, 1);

            UNIT_TEST(areEqual(maxIndex - codeLength / 2, codePosition));
        }

        static double speedFindCode(unsigned long testCount) {
            Eigen::ArrayXXi bitSequence(1, 1);
            Eigen::MatioFile file2("testfiles/newMask12Bits_x3_2piNormalized.mat", MAT_ACC_RDONLY);
            file2.read_mat("bit_sequence_codee", bitSequence);

            Eigen::ArrayXXd codeSample(56, 1);
            AbsoluteDecoding decode;
            decode.resize(bitSequence);
            codeSample << 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0;
            codeSample.transposeInPlace();
            tic();
            for (unsigned long i = 0; i < testCount; i++) {
                decode.findCodePosition(codeSample, 1);
            }

            return toc(testCount);
        }
    };
}
#endif // !TESTABSOLUTEDECODING_HPP