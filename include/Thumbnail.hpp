/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef THUMBNAIL_HPP
#define THUMBNAIL_HPP

#include "Common.hpp"
#include "Plane.hpp"
#include "Cell.hpp"

namespace vernier {

    /** \brief Uses a given periodic array and its phases planes, computes the thumbnail used as a model-reduction*/
    class Thumbnail {
    private:
        int orderMin1, orderMin2;
        int length1, length2;
        Eigen::VectorXd codeOrientation;
        Eigen::ArrayXXd numberWhiteDots, cumulWhiteDots, numberBackgroundDots, cumulBackgroundDots;
        Eigen::VectorXd sequence1, sequence2;
        Eigen::VectorXd numberWhiteRefDots1, cumulWhiteRefDots1, numberBackRefDots1, cumulBackRefDots1, numberCodingDots1, cumulCodingDots1, meanCodingDots1, meanBackRefDots1, meanWhiteRefDots1;
        Eigen::VectorXd numberWhiteRefDots2, cumulWhiteRefDots2, numberBackRefDots2, cumulBackRefDots2, numberCodingDots2, cumulCodingDots2, meanCodingDots2, meanBackRefDots2, meanWhiteRefDots2;
        int MSB1, MSB2;
        Cell cell;

    public:
        Eigen::ArrayXXd codeIntensity1, codeIntensity2;

        Thumbnail();

        /** Access to the number of cells contained in the image pattern with the plane coefficients
         *	and the size of the image pattern
         *
         *	\param plane: conseidered plane for the thumbnail size computation
         *	\param nRows: number of rows of the image pattern
         *	\param ncols: number of columns of the image pattern
         */
        int getLength(Plane plane, int nRows, int nCols);

        /** Resizes every array and vector used for the later computation of the thumbnail
         *
         *	\param length1: length along one axis of the thumbnail (we can't know which one in advance)
         *	\param length2: length along the other axis of the thumbail
         */
        void resize(int length1, int length2);

        /** Goes through the whole thumbnail and search for the binary coding values of the pattern*/
        void getCodeSequence();

        /**Computes the thumbnail and fill the arrays and vectors prepared in the resize method
         *
         *	\param plane1: first plane of the unwrapped phase (in the first direction)
         *	\param plane2: second plane of the unwrapped phase (in the second direction)
         *	\param patternArray: array that will receive the thumbnail
         */
        void compute(Plane plane1, Plane plane2, const Eigen::ArrayXXd& patternArray);

        /** Computes the thumbnail and stores it in an array
         *	This method is used in intern for the compute function
         *
         *	\param plane1: first plane of the unwrapped phase (in the first direction)
         *	\param plane2: second plane of the unwrapped phase (in the second direction)
         *	\param patternArray: array that will receive the thumbnail
         */
        void computeThumbnail(Plane plane1, Plane plane2, const Eigen::ArrayXXd& patternArray, double deltaPhase = PI / 3);

        void computeThumbnailTotal(Plane plane1, Plane plane2, const Eigen::ArrayXXd& patternArray, double deltaPhase);


        Eigen::ArrayXXd getThumbnailArray();

        Eigen::ArrayXXd getBackgroundForegroundArray();

        Eigen::ArrayXXd getMeanDots();

        /** Graphical feedback of thumbnail*/

        /** Method used to display the thumbnail*/
        cv::Mat getMeanDotsImage(int rectWidth = 10);
        
        void guiMeanDots(cv::Mat& thumbnailImg);

        void drawThumbnailDetection(cv::Mat& thumbnailImg);

        /** Method used to display the coded sequence in regard of the thumbnail size*/
        void showSequences(cv::Mat& sequencesBiDir);

        void drawSequence1Level(Eigen::ArrayXXd& codingLevelSecurity1, Eigen::ArrayXXd& codingLevelSecurity2);

        /** Method used to display the coding cell and the code orientation*/
        void showDisplayCode();

        /** Displays with an arrow the orientation of the code ans where to pinpoint the position (0,0)*/
        void showCodeDirection();

        Eigen::VectorXd getSequence1();

        Eigen::VectorXd getSequence2();

        Eigen::VectorXd getCodeOrientation();

        int getMSB1();

        int getMSB2();

        Eigen::ArrayXXd getNumberWhiteDots();

        Eigen::ArrayXXd getCumulWhiteDots();

        Eigen::ArrayXXd getNumberBackground();

        Eigen::ArrayXXd getCumulBackground();

        void removeNanFromArray(Eigen::ArrayXXd& image);

        Eigen::ArrayXXd getNumberBackDots();

        Eigen::ArrayXXd getCumulBackDots();


        void drawCodeDetection(cv::Mat& image);

        double computeSTD2(Eigen::ArrayXXd image);
        
        void rotate90();
        void rotate180();
        void rotate270();
    };
}
#endif // !THUMBNAIL_HPP