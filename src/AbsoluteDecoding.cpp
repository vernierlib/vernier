/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "AbsoluteDecoding.hpp"

namespace Vernier {

    AbsoluteDecoding::AbsoluteDecoding() {
    }

    AbsoluteDecoding::AbsoluteDecoding(Eigen::ArrayXXi& bitSequence) {
        resize(bitSequence);
    }

    void AbsoluteDecoding::resize(Eigen::ArrayXXi& bitSequence) {
        for (int i = 0; i < bitSequence.cols(); i++) {
            if (bitSequence(0, i) == 0) {
                bitSequence(0, i) = -1;
            }
            if (i % 3 == 0 || i % 3 == 2) {
                bitSequence(0, i) = 0;
            }
        }
        this->bitSequence = bitSequence;
    }

    Eigen::ArrayXXd AbsoluteDecoding::getCodeSequence(Eigen::ArrayXXd numberWhiteDots, Eigen::ArrayXXd cumulWhiteDots, Eigen::ArrayXXd numberBackgroundDots, Eigen::ArrayXXd cumulBackgroundDots, Eigen::VectorXd& codeOrientation) {
        Eigen::ArrayXXd sequence;
        sequence = Eigen::ArrayXXd::Zero(std::max(numberWhiteDots.rows(), numberWhiteDots.cols()), 2);

        int coding1 = codeOrientation(0);
        int coding2 = codeOrientation(1);
        int missing1 = codeOrientation(2);
        int missing2 = codeOrientation(3);

        int startIndex1 = 0;
        int startIndex2 = 0;
        int stopIndex1 = numberWhiteDots.rows() - 1;
        int stopIndex2 = numberWhiteDots.cols() - 1;

        if (coding1 == 0) {
            startIndex1 = 1;
        }

        if (numberWhiteDots.rows() % 3 == coding1) {
            stopIndex1 = numberWhiteDots.rows() - 2;
        }

        if (coding2 == 0) {
            startIndex2 = 1;
        }

        if (numberWhiteDots.cols() % 3 == coding2) {
            stopIndex2 = numberWhiteDots.cols() - 2;
        }

        Eigen::VectorXd numberWhiteRefDots = Eigen::VectorXd::Zero(numberWhiteDots.rows());
        Eigen::VectorXd cumulWhiteRefDots = Eigen::VectorXd::Zero(numberWhiteDots.rows());
        Eigen::VectorXd numberBackRefDots = Eigen::VectorXd::Zero(numberWhiteDots.rows());
        Eigen::VectorXd cumulBackRefDots = Eigen::VectorXd::Zero(numberWhiteDots.rows());
        Eigen::VectorXd numberCodingDots = Eigen::VectorXd::Zero(numberWhiteDots.rows());
        Eigen::VectorXd cumulCodingDots = Eigen::VectorXd::Zero(numberWhiteDots.rows());
        Eigen::VectorXd meanCodingDots = Eigen::VectorXd::Zero(numberWhiteDots.rows());
        Eigen::VectorXd meanBackRefDots = Eigen::VectorXd::Zero(numberWhiteDots.rows());
        Eigen::VectorXd meanWhiteRefDots = Eigen::VectorXd::Zero(numberWhiteDots.rows());

        // sequence 1
        for (int index1 = startIndex1; index1 < stopIndex1; index1++) {
            if (index1 % 3 != coding1 % 3) {
                sequence(index1, 0) = 0;
            } else {
                for (int index2 = startIndex2; index2 < stopIndex2; index2++) {
                    numberBackRefDots(index1) += numberBackgroundDots(index1, index2);
                    cumulBackRefDots(index1) += cumulBackgroundDots(index1, index2);
                    // Ici on a deux point sur trois dans une direction qui sont cosid�r�s comme non codants
                    if (index2 % 3 != coding2 % 3) {
                        numberCodingDots(index1) += numberWhiteDots(index1, index2);
                        cumulCodingDots(index1) += cumulWhiteDots(index1, index2);

                        if ((index1 - 1) % 3 != missing1 % 3 || index2 % 3 != missing2 % 3) {
                            numberWhiteRefDots(index1) += numberWhiteDots(index1 - 1, index2);
                            cumulWhiteRefDots(index1) += cumulWhiteDots(index1 - 1, index2);
                        }
                        if ((index1 + 1) % 3 != missing1 % 3 || index2 % 3 != missing2 % 3) {
                            numberWhiteRefDots(index1) += numberWhiteDots(index1 + 1, index2);
                            cumulWhiteRefDots(index1) += cumulWhiteDots(index1 + 1, index2);
                        }
                    }
                }

                meanCodingDots(index1) = cumulCodingDots(index1) / numberCodingDots(index1);
                meanBackRefDots(index1) = cumulBackRefDots(index1) / numberBackRefDots(index1);
                meanWhiteRefDots(index1) = cumulWhiteRefDots(index1) / numberWhiteRefDots(index1);

                if (abs(meanCodingDots(index1) - meanBackRefDots(index1)) < abs(meanWhiteRefDots(index1) - meanCodingDots(index1))) {
                    sequence(index1, 0) = -1;
                } else {
                    sequence(index1, 0) = 1;
                }
            }
        }

        numberWhiteRefDots = Eigen::VectorXd::Zero(numberWhiteDots.cols());
        cumulWhiteRefDots = Eigen::VectorXd::Zero(numberWhiteDots.cols());
        numberBackRefDots = Eigen::VectorXd::Zero(numberWhiteDots.cols());
        cumulBackRefDots = Eigen::VectorXd::Zero(numberWhiteDots.cols());
        numberCodingDots = Eigen::VectorXd::Zero(numberWhiteDots.cols());
        cumulCodingDots = Eigen::VectorXd::Zero(numberWhiteDots.cols());
        meanCodingDots = Eigen::VectorXd::Zero(numberWhiteDots.cols());
        meanBackRefDots = Eigen::VectorXd::Zero(numberWhiteDots.cols());
        meanWhiteRefDots = Eigen::VectorXd::Zero(numberWhiteDots.cols());

        // sequence 2
        for (int index2 = startIndex2; index2 < stopIndex2; index2++) {
            if (index2 % 3 != coding2 % 3) {
                sequence(index2, 1) = 0;
            } else {
                for (int index1 = startIndex1; index1 < stopIndex1; index1++) {
                    numberBackRefDots(index2) += numberBackgroundDots(index1, index2);
                    cumulBackRefDots(index2) += cumulBackgroundDots(index1, index2);
                    // Ici on a deux point sur trois dans une direction qui sont cosid�r�s comme non codants
                    if (index1 % 3 != coding1 % 3) {
                        numberCodingDots(index2) += numberWhiteDots(index1, index2);
                        cumulCodingDots(index2) += cumulWhiteDots(index1, index2);

                        if (index2 - 1 % 3 != missing2 % 3 || index1 % 3 != missing1 % 3) {
                            numberWhiteRefDots(index2) += numberWhiteDots(index1, index2 - 1);
                            cumulWhiteRefDots(index2) += cumulWhiteDots(index1, index2 - 1);
                        }
                        if (index2 + 1 % 3 != missing2 % 3 || index1 % 3 != missing1 % 3) {
                            numberWhiteRefDots(index2) += numberWhiteDots(index1, index2 + 1);
                            cumulWhiteRefDots(index2) += cumulWhiteDots(index1, index2 + 1);
                        }
                    }
                }

                meanCodingDots(index2) = cumulCodingDots(index2) / numberCodingDots(index2);
                meanBackRefDots(index2) = cumulBackRefDots(index2) / numberBackRefDots(index2);
                meanWhiteRefDots(index2) = cumulWhiteRefDots(index2) / numberWhiteRefDots(index2);

                if (abs(meanCodingDots(index2) - meanBackRefDots(index2)) < abs(meanWhiteRefDots(index2) - meanCodingDots(index2))) {
                    sequence(index2, 1) = -1;
                } else {
                    sequence(index2, 1) = 1;
                }
            }
        }

        return sequence;
    }

    int AbsoluteDecoding::findCodePosition(Eigen::ArrayXXd& codeSample, int MSB) {
        double maximum;
        int offset = floor(codeSample.rows() / 2);
        int direction = 1;

        if (MSB == 0) {
            codeSample.colwise().reverseInPlace();
            direction = -1;
        }

        //bitSequence = bitSequence.block(0, 0, 1, 100);

        // convolution with 'same' mode
        Eigen::ArrayXXd testPeak(1, bitSequence.cols());
        for (int i = 0; i < bitSequence.cols(); i++) {
            int jMin;
            int jMax;
            if (i >= offset) {
                jMin = 0;
            } else {
                jMin = offset - i;
            }

            if (i + offset < bitSequence.cols()) {
                jMax = codeSample.rows() - 1;
            } else {
                jMax = bitSequence.cols() - 1 - i + offset;
            }

            testPeak(0, i) = 0;
            for (int j = jMin; j <= jMax; j++) {
                testPeak(0, i) += (double) bitSequence(0, i - offset + j) * codeSample(j, 0);
            }
        }

        Eigen::MatrixXd::Index maxRow, maxCol;

        maximum = testPeak.maxCoeff(&maxRow, &maxCol);

        return direction * maxCol;
    }
}