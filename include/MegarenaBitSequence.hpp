/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2025 CNRS, ENSMM, UFC.
 */

#ifndef MEGARENABITSEQUENCE_HPP
#define MEGARENABITSEQUENCE_HPP

#include "Common.hpp"

namespace vernier {

    class MegarenaBitSequence {
    public:

        static void generate(int codeDepth, Eigen::ArrayXXi & sequence);
        static bool check(int codeDepth, Eigen::ArrayXXi & bs);
        static int codeDepth(int sequenceLength);

    private:
        static int getBit(int state, int bitPosition);
        static int nextBit(int codeDepth, int state);


    };

}

#endif