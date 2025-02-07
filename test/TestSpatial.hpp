/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTSPATIAL_HPP
#define TESTSPATIAL_HPP

#include "Vernier.hpp"
#include "UnitTest.hpp"

#include "Spatial.hpp"
#include "eigen-matio/MatioFile.hpp"


namespace vernier {
	/** \brief  Computes the tests of the Unwrapping class in order to validate this one quickly and systematicly
	*	This class is made of three main functions in order to keep the process simple and repeatable at each
	*	new version of the vernier library.
	*/

	class TestSpatial {
	public:
		/** Creates a simple wrapped matrix in order to display in the console the result.
		*	Displays :
		*	- the input wrapped phase (a 4x8 matrix)
		*	- the output unwrapped matrix
		*/
		static void main() {
			// construct a 4x8 wrapped phase

			Eigen::ArrayXXd wrappedPhase(6, 8);
			wrappedPhase << 0, 1, 2, 3, -3, -2, -1, 0,
				0, 1, 2, 3, -3, -2, -1, 0,
				0, 1, 2, 3, -3, -2, -1, 0,
				0, 1, 2, 3, -3, -2, -1, 0,
				0, 1, 2, 3, -3, -2, -1, 0,
				0, 1, 2, 3, -3, -2, -1, 0;

			std::cout << wrappedPhase << "\n\n\n" << std::endl;

			Spatial::quartersUnwrapPhase(wrappedPhase);

			std::cout << wrappedPhase << "\n\n\n" << std::endl;
		}

		static void main2() {
#ifdef USE_OPENCV			
			Eigen::ArrayXXcd patternMatrix;
			Eigen::ArrayXXd stripeSorting;
			stripeSorting = Eigen::ArrayXXd::Zero(3, 1024);
			double deltaPhase = PI / 6;
			Eigen::ArrayXXd wrappedPhasePeak1(1, 1);

			cv::Mat image = cv::imread("testfiles/im_1.png");
			Eigen::MatrixXd intermediaryMatrix;
			cv::cv2eigen(image, intermediaryMatrix);
			patternMatrix = intermediaryMatrix.array();

			Eigen::MatioFile file("testfiles/TestFilesMat.mat", MAT_ACC_RDWR);
			file.read_mat("wrappedPhasePeak1", wrappedPhasePeak1);

			Spatial::quartersUnwrapPhase(wrappedPhasePeak1);
#endif //USE_OPENCV
		}

		/**	Run the routine made to read a wrapped phase coming from a matlab file
		*	The function is reading two wrapped phase (in two perpendicular directions)
		*
		*	Displays the two unwrapped phase map corresponding to the two directions wrapped phase maps
		*/
		static void runAllTests() {
			Eigen::ArrayXXd wrappedPhasePeak1(1, 1);
			Eigen::ArrayXXd unwrappedReference(1, 1);
			Eigen::MatioFile file("testfiles/TestFilesMat.mat", MAT_ACC_RDWR);
			file.read_mat("wrappedPhasePeak1", wrappedPhasePeak1);

			Spatial::quartersUnwrapPhase(wrappedPhasePeak1);
			file.read_mat("unwrapCenterMatlab", unwrappedReference);

			UNIT_TEST(areEqual(unwrappedReference, wrappedPhasePeak1));
		}

		/* Runs a given amount of times the unwrapping function
		*
		*	\params testCount: number of times the function quartersUnwrapping will run
		*/
		static double speed(unsigned long testCount) {
			Eigen::ArrayXXd wrappedPhasePeak1(1, 1);
			Eigen::MatioFile file("testfiles/TestFilesMat.mat");
			file.read_mat("wrappedPhasePeak1", wrappedPhasePeak1);

			tic();

			for (unsigned long i = 0; i < testCount; i++) {
				Spatial::quartersUnwrapPhase(wrappedPhasePeak1);
			}

			return toc(testCount);
		}
	};
}

#endif