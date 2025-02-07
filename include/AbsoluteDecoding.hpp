/*
 * This file is part of VERNIER Library.
 *
 * Copyright (c) 2018-2020 UBFC, ENSMM, UFC, CNRS.
 *
 * Authors: Antoine N. ANDRE, Guillaume J. LAURENT, Patrick SANDOZ
 *
 */

#ifndef ABSOLUTEDECODING_HPP
#define ABSOLUTEDECODING_HPP

#include "Common.hpp"

namespace vernier {

    /** \brief Finds the place of the coding sample extracted from the pattern in the full coded sequence. **/
    class AbsoluteDecoding {
    private:
        Eigen::ArrayXXi bitSequence;
        Eigen::Array33d sumOnlyDotsRemain;

    public:

        /** Construct an empty constructor*/
        AbsoluteDecoding();

        /** Constructs the decoding class with the complete coded sequence
         *
         *	\param bitSequence: ArrayXXd containing the bit coded sequence
         */
        AbsoluteDecoding(Eigen::ArrayXXi& bitSequence);

        /** resize the decoding class with the complete coded sequence
         *
         *	\param bitSequence: ArrayXXd containing the bit coded sequence
         */
        void resize(Eigen::ArrayXXi& bitSequence);

        /** Get the two coded sequence binarized with the different array of dots
         *	\param numberWhiteDots : array containing the number of white dots of the pattern
         *	\param cumulWhiteDots : array containing the cumul of intensity of white dots of the pattern
         *	\param numberBackgroundDots : array containing the number of bacckground dots of the pattern
         *	\param cumulBackgroundDots : array containing the cumul of intensity of background dots of the pattern
         *	\param codeOrientation : vector containing the orientation of the code of the pattern
         *		int coding1 = codeOrientation(0);
         *		int coding2 = codeOrientation(1);
         *		int missing1 = codeOrientation(2);
         *		int missing2 = codeOrientation(3);
         *
         */
        Eigen::ArrayXXd getCodeSequence(Eigen::ArrayXXd numberWhiteDots, Eigen::ArrayXXd cumulWhiteDots, Eigen::ArrayXXd numberBackgroundDots, Eigen::ArrayXXd cumulBackgroundDots, Eigen::VectorXd& codeOrientation);

        /** Finds where the sample coming from the pattern analysis fits in the complete coded sequence
         *
         *	\param codingSample: sample coding coming from the pattern analysis
         */
        int findCodePosition(Eigen::ArrayXXd& codeSample, int MSB);
    };
}
#endif // !ABSOLUTEDECODING_HPP