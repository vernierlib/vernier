/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UFC.
 */

#include <math.h>

#include "MegarenaBitSequence.hpp"

namespace vernier {

    int MegarenaBitSequence::getBit(int state, int bitPosition) {
        return (state >> bitPosition) & 1;
    }

    int MegarenaBitSequence::nextBit(int codeDepth, int state) {
        if (codeDepth == 4) {
            return (getBit(state, 0) + getBit(state, 3)) % 2;
        } else if (codeDepth == 5) {
            return (getBit(state, 1) + getBit(state, 4)) % 2;
        } else if (codeDepth == 6) {
            return (getBit(state, 0) + getBit(state, 5)) % 2;
        } else if (codeDepth == 7) {
            return (getBit(state, 2) + getBit(state, 6)) % 2;
        } else if (codeDepth == 8) {
            return (getBit(state, 0) + getBit(state, 1) + getBit(state, 6) + getBit(state, 7)) % 2;
        } else if (codeDepth == 9) {
            return (getBit(state, 3) + getBit(state, 8)) % 2;
        } else if (codeDepth == 10) {
            return (getBit(state, 6) + getBit(state, 9)) % 2;
        } else if (codeDepth == 11) {
            return (getBit(state, 1) + getBit(state, 10)) % 2;
        } else if (codeDepth == 12) {
            return (getBit(state, 0) + getBit(state, 1) + getBit(state, 7) + getBit(state, 11)) % 2;
        } else {
            throw Exception("The megarena code depth must between 4 and 12.");
        }
    }
    
    int MegarenaBitSequence::codeDepth(int sequenceLength) {
        return std::round(std::log2(sequenceLength/3));
    }

    void MegarenaBitSequence::generate(int codeDepth, Eigen::ArrayXXi & sequence) {
        int codeMax = 1 << codeDepth; // 2^codeDepth
        int codeCount = codeMax - 1;

        Eigen::ArrayXXi bit = Eigen::ArrayXXi::Ones(1, codeCount);
        Eigen::ArrayXXi code = Eigen::ArrayXXi::Zero(1, codeCount);
        code(0) = codeCount;
        for (int index = 1; index < codeCount; index++) {
            int state = code(index - 1);
            bit(index) = nextBit(codeDepth, state);
            code(index) = (code(index - 1) * 2) % codeMax + bit(index);
        }

        sequence.setOnes(1, 3 * (codeCount + codeDepth - 1));
        for (int index = 0; index < codeCount - 1; index++) {
            sequence(3 * (index + codeDepth - 1) + 1) = bit(index);
        }
    }

    bool MegarenaBitSequence::check(int codeDepth, Eigen::ArrayXXi & bs) {
        int codeMax = 1 << codeDepth; // 2^codeDepth
        int codeCount = codeMax - 1;

        Eigen::ArrayXXi bit_nb = Eigen::ArrayXXi::Ones(1, codeCount);
        for (int index = 0; index < codeCount - 1; index++) {
            bit_nb(index) = bs(3 * (index + codeDepth - 1) + 1);
        }

        Eigen::ArrayXXi code = Eigen::ArrayXXi::Zero(1, codeCount);
        code(0) = codeCount;
        for (int index = 1; index < codeCount; index++) {
            code(index) = (code(index - 1) * 2) % codeMax + bit_nb(index);
        }

        Eigen::ArrayXXi check = Eigen::ArrayXXi::Zero(1, codeCount);
        for (int index = 0; index < codeCount; index++) {
            check(code(index) - 1) += 1;
        }

        int index = 0;
        while (index < codeCount && check(index) == 1) {
            index++;
        }
        return (index == codeCount);
    }

}