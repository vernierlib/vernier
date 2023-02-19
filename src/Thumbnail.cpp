/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "Thumbnail.hpp"
#include <iomanip>

namespace Vernier {

    Thumbnail::Thumbnail() {
    }

    int Thumbnail::getLength(Plane plane, int nRows, int nCols) {
        double stripesValues[4] = {plane.getC(), plane.getA() * nCols + plane.getC(), plane.getB() * nRows + plane.getC(), plane.getA() * nCols + plane.getB() * nRows + plane.getC()};
        double nMin = *std::min_element(stripesValues, stripesValues + 4);
        nMin = nMin / (2 * PI);
        double nMax = *std::max_element(stripesValues, stripesValues + 4);
        nMax = nMax / (2 * PI);

        return abs(floor(nMax)) + abs(floor(nMin)) + 1;
    }

    void Thumbnail::resize(int length1, int length2) {
        this->length1 = length1;
        this->length2 = length2;

        //for the thumbnail
        numberWhiteDots.resize(length1, length2);
        cumulWhiteDots.resize(length1, length2);
        numberBackgroundDots.resize(length1, length2);
        cumulBackgroundDots.resize(length1, length2);

        numberWhiteDots.fill(0);
        cumulWhiteDots.fill(0);
        numberBackgroundDots.fill(0);
        cumulBackgroundDots.fill(0);

        //for the coded sequences
        numberWhiteRefDots1.resize(length1);
        cumulWhiteRefDots1.resize(length1);
        numberBackRefDots1.resize(length1);
        cumulBackRefDots1.resize(length1);
        numberCodingDots1.resize(length1);
        cumulCodingDots1.resize(length1);
        meanCodingDots1.resize(length1);
        meanBackRefDots1.resize(length1);
        meanWhiteRefDots1.resize(length1);

        numberWhiteRefDots1.fill(0);
        cumulWhiteRefDots1.fill(0);
        numberBackRefDots1.fill(0);
        cumulBackRefDots1.fill(0);
        numberCodingDots1.fill(0);
        cumulCodingDots1.fill(0);
        meanCodingDots1.fill(0);
        meanBackRefDots1.fill(0);
        meanWhiteRefDots1.fill(0);

        numberWhiteRefDots2.resize(length2);
        cumulWhiteRefDots2.resize(length2);
        numberBackRefDots2.resize(length2);
        cumulBackRefDots2.resize(length2);
        numberCodingDots2.resize(length2);
        cumulCodingDots2.resize(length2);
        meanCodingDots2.resize(length2);
        meanBackRefDots2.resize(length2);
        meanWhiteRefDots2.resize(length2);

        numberWhiteRefDots2.fill(0);
        cumulWhiteRefDots2.fill(0);
        numberBackRefDots2.fill(0);
        cumulBackRefDots2.fill(0);
        numberCodingDots2.fill(0);
        cumulCodingDots2.fill(0);
        meanCodingDots2.fill(0);
        meanBackRefDots2.fill(0);
        meanWhiteRefDots2.fill(0);

        //create the actual sequences
        sequence1.resize(length1);
        sequence1.fill(0);

        sequence2.resize(length2);
        sequence2.fill(0);
    }

    void Thumbnail::getCodeSequence() {
        int coding1 = codeOrientation(0);
        int coding2 = codeOrientation(1);
        int missing1 = codeOrientation(2);
        int missing2 = codeOrientation(3);

        int startIndex1 = 0;
        int startIndex2 = 0;
        int stopIndex1 = numberWhiteDots.rows();
        int stopIndex2 = numberWhiteDots.cols();

        if (coding1 == 0) {
            startIndex1 = 1;
            sequence1(0) = 0;
        }

        if (numberWhiteDots.rows() % 3 == coding1) {
            stopIndex1 = numberWhiteDots.rows() - 1;
            sequence1(numberWhiteDots.rows() - 1) = 0;
        }

        if (coding2 == 0) {
            startIndex2 = 1;
            sequence2(0) = 0;
        }

        if (numberWhiteDots.cols() % 3 == coding2) {
            stopIndex2 = numberWhiteDots.cols() - 1;
            sequence2(numberWhiteDots.cols() - 1) = 0;
        }

        codeIntensity1.resize(stopIndex1 - startIndex1, 3);
        codeIntensity2.resize(stopIndex2 - startIndex2, 3);
        codeIntensity1.setConstant(0.0);
        codeIntensity2.setConstant(0.0);

        // sequence 1

        //Pour contraste:
        Eigen::VectorXd contrastVec1;

        for (int index1 = startIndex1; index1 < stopIndex1; index1++) {
            if (index1 % 3 != coding1 % 3) {
                sequence1(index1) = 0;
            } else {
                for (int index2 = startIndex2; index2 < stopIndex2; index2++) {
                    numberBackRefDots1(index1) += numberBackgroundDots(index1, index2);
                    cumulBackRefDots1(index1) += cumulBackgroundDots(index1, index2);
                    // Ici on a deux point sur trois dans une direction qui sont cosid�r�s comme non codants
                    if (index2 % 3 != coding2 % 3) {
                        numberCodingDots1(index1) += numberWhiteDots(index1, index2);
                        cumulCodingDots1(index1) += cumulWhiteDots(index1, index2);

                        if ((index1 - 1) % 3 != missing1 % 3 || index2 % 3 != missing2 % 3) {
                            numberWhiteRefDots1(index1) += numberWhiteDots(index1 - 1, index2);
                            cumulWhiteRefDots1(index1) += cumulWhiteDots(index1 - 1, index2);
                        }
                        if (((index1 + 1) % 3 != missing1 % 3 || index2 % 3 != missing2 % 3) && index1 < numberWhiteRefDots1.rows() - 2) {
                            numberWhiteRefDots1(index1) += numberWhiteDots(index1 + 1, index2);
                            cumulWhiteRefDots1(index1) += cumulWhiteDots(index1 + 1, index2);
                        }
                    }
                }

                meanCodingDots1(index1) = cumulCodingDots1(index1) / numberCodingDots1(index1);
                meanBackRefDots1(index1) = cumulBackRefDots1(index1) / numberBackRefDots1(index1);
                meanWhiteRefDots1(index1) = cumulWhiteRefDots1(index1) / numberWhiteRefDots1(index1);

                codeIntensity1(index1, 0) = meanCodingDots1(index1);
                codeIntensity1(index1, 1) = meanBackRefDots1(index1);
                codeIntensity1(index1, 2) = meanWhiteRefDots1(index1);

                contrastVec1.conservativeResize(contrastVec1.rows() + 1, contrastVec1.cols());

                contrastVec1(contrastVec1.rows() - 1) = (codeIntensity1(index1, 2) - codeIntensity1(index1, 1));

                //std::cout << (codeIntensity1(index1, 2) - codeIntensity1(index1, 1)) / 256.0 << std::endl;

                if (abs(meanCodingDots1(index1) - meanBackRefDots1(index1)) < abs(meanWhiteRefDots1(index1) - meanCodingDots1(index1))) {
                    sequence1(index1) = -1;
                } else {
                    sequence1(index1) = 1;
                }
            }
        }

        // sequence 2

        //Pour contraste:
        Eigen::VectorXd contrastVec2;

        for (int index2 = startIndex2; index2 < stopIndex2; index2++) {
            if (index2 % 3 != coding2 % 3) {
                sequence2(index2) = 0;
            } else {
                for (int index1 = startIndex1; index1 < stopIndex1; index1++) {
                    numberBackRefDots2(index2) += numberBackgroundDots(index1, index2);
                    cumulBackRefDots2(index2) += cumulBackgroundDots(index1, index2);
                    // Ici on a deux point sur trois dans une direction qui sont cosid�r�s comme non codants
                    if (index1 % 3 != coding1 % 3) {
                        numberCodingDots2(index2) += numberWhiteDots(index1, index2);
                        cumulCodingDots2(index2) += cumulWhiteDots(index1, index2);

                        if ((index2 - 1) % 3 != missing2 % 3 || index1 % 3 != missing1 % 3) {
                            numberWhiteRefDots2(index2) += numberWhiteDots(index1, index2 - 1);
                            cumulWhiteRefDots2(index2) += cumulWhiteDots(index1, index2 - 1);
                        }
                        if (((index2 + 1) % 3 != missing2 % 3 || index1 % 3 != missing1 % 3) && index2 < numberWhiteRefDots2.rows() - 2) {
                            numberWhiteRefDots2(index2) += numberWhiteDots(index1, index2 + 1);
                            cumulWhiteRefDots2(index2) += cumulWhiteDots(index1, index2 + 1);
                        }
                    }
                }

                meanCodingDots2(index2) = cumulCodingDots2(index2) / numberCodingDots2(index2);
                meanBackRefDots2(index2) = cumulBackRefDots2(index2) / numberBackRefDots2(index2);
                meanWhiteRefDots2(index2) = cumulWhiteRefDots2(index2) / numberWhiteRefDots2(index2);

                codeIntensity2(index2, 0) = meanCodingDots2(index2);
                codeIntensity2(index2, 1) = meanBackRefDots2(index2);
                codeIntensity2(index2, 2) = meanWhiteRefDots2(index2);

                contrastVec2.conservativeResize(contrastVec2.rows() + 1, contrastVec2.cols());
                contrastVec2(contrastVec2.rows() - 1) = (codeIntensity2(index2, 2) - codeIntensity2(index2, 1));

                //std::cout << (codeIntensity2(index2, 2) - codeIntensity2(index2, 1)) / 256.0 << std::endl;

                if (abs(meanCodingDots2(index2) - meanBackRefDots2(index2)) < abs(meanWhiteRefDots2(index2) - meanCodingDots2(index2))) {
                    sequence2(index2) = -1;
                } else {
                    sequence2(index2) = 1;
                }
            }
        }
    }

    void Thumbnail::compute(Plane plane1, Plane plane2, Eigen::ArrayXXd& patternArray) {
        computeThumbnail(plane1, plane2, patternArray, PI / 4.0);

        cell.getGlobalCell(numberWhiteDots, cumulWhiteDots);

        this->codeOrientation = cell.getCodeOrientation();

        //cv::Mat codingCellImage(10, 10, CV_64FC3);
        //cell.guiDisplayCell(codingCellImage);
        //cv::imshow("coding cell", codingCellImage);
        getCodeSequence();

        int coding1 = codeOrientation(0);
        int coding2 = codeOrientation(1);
        int missing1 = codeOrientation(2);
        int missing2 = codeOrientation(3);

        if ((missing1 + 1) % 3 == coding1) {
            MSB1 = 1;
        } else {
            MSB1 = 0;
        }
        if ((missing2 + 1) % 3 == coding2) {
            MSB2 = 1;
        } else {
            MSB2 = 0;
        }
    }

    void Thumbnail::computeThumbnail(Plane plane1, Plane plane2, Eigen::ArrayXXd& patternArray, double deltaPhase) {
        //this method is used in intern to save space and time

        int phaseIteration1, phaseIteration2;

        Eigen::Vector3d tempPlane1Coeff(plane1.getA(), plane1.getB(), plane1.getC());
        Eigen::Vector3d tempPlane2Coeff(plane2.getA(), plane2.getB(), plane2.getC());

        Plane tempPlane1(tempPlane1Coeff);
        Plane tempPlane2(tempPlane2Coeff);


        for (int col = 0; col < patternArray.cols(); col++) {
            for (int row = 0; row < patternArray.rows(); row++) {
                //double phaseCol = plane1.getPhase(row, col);
                //double phaseRow = plane2.getPhase(row, col);
                double phaseCol = tempPlane1.getPhase(row - patternArray.rows() / 2, col - patternArray.cols() / 2);
                double phaseRow = tempPlane2.getPhase(row - patternArray.rows() / 2, col - patternArray.cols() / 2);

                phaseIteration1 = round(phaseCol / (2.0 * PI)) + length1 / 2;
                phaseIteration2 = round(phaseRow / (2.0 * PI)) + length2 / 2;

                if (phaseIteration1 < cumulBackgroundDots.rows() && phaseIteration2 < cumulBackgroundDots.cols() && phaseIteration1 >= 0 && phaseIteration2 >= 0) {
                    if ((abs(std::fmod(phaseCol, 2 * PI)) <= deltaPhase || abs(std::fmod(phaseCol, 2 * PI)) >= 2 * PI - deltaPhase) && (abs(std::fmod(phaseRow, 2 * PI)) <= deltaPhase || abs(std::fmod(phaseRow, 2 * PI)) >= 2 * PI - deltaPhase)) {
                        numberWhiteDots(phaseIteration1, phaseIteration2) += 1;
                        cumulWhiteDots(phaseIteration1, phaseIteration2) += patternArray(row, col);
                    } else if ((abs(std::fmod(phaseCol, 2 * PI)) >= PI - deltaPhase && abs(std::fmod(phaseCol, 2 * PI)) <= PI + deltaPhase) || (abs(std::fmod(phaseRow, 2 * PI)) >= PI - deltaPhase && abs(std::fmod(phaseRow, 2 * PI)) <= PI + deltaPhase)) {
                        numberBackgroundDots(phaseIteration1, phaseIteration2) += 1;
                        cumulBackgroundDots(phaseIteration1, phaseIteration2) += patternArray(row, col);
                    }
                }
            }
        }
    }

    void Thumbnail::computeThumbnailTotal(Plane plane1, Plane plane2, Eigen::ArrayXXd& patternArray, double deltaPhase) {
        //this method is used in intern to save space and time

        int phaseIteration1, phaseIteration2;

        Eigen::Vector3d tempPlane1Coeff(plane1.getA() * 2.0, plane1.getB() * 2.0, plane1.getC() * 2.0);
        Eigen::Vector3d tempPlane2Coeff(plane2.getA() * 2.0, plane2.getB() * 2.0, plane2.getC() * 2.0);

        Plane tempPlane1(tempPlane1Coeff);
        Plane tempPlane2(tempPlane2Coeff);


        for (int col = 0; col < patternArray.cols(); col++) {
            for (int row = 0; row < patternArray.rows(); row++) {
                //double phaseCol = plane1.getPhase(row, col);
                //double phaseRow = plane2.getPhase(row, col);
                double phaseCol = tempPlane1.getPhase(row - patternArray.rows() / 2, col - patternArray.cols() / 2);
                double phaseRow = tempPlane2.getPhase(row - patternArray.rows() / 2, col - patternArray.cols() / 2);

                phaseIteration1 = round(phaseCol / (2.0 * PI)) + length1 / 2;
                phaseIteration2 = round(phaseRow / (2.0 * PI)) + length2 / 2;

                if (phaseIteration1 < cumulBackgroundDots.rows() && phaseIteration2 < cumulBackgroundDots.cols() && phaseIteration1 >= 0 && phaseIteration2 >= 0) {
                    if ((abs(std::fmod(phaseCol, 2 * PI)) <= deltaPhase || abs(std::fmod(phaseCol, 2 * PI)) >= 2 * PI - deltaPhase) && (abs(std::fmod(phaseRow, 2 * PI)) <= deltaPhase || abs(std::fmod(phaseRow, 2 * PI)) >= 2 * PI - deltaPhase)) {
                        numberWhiteDots(phaseIteration1, phaseIteration2) += 1;
                        cumulWhiteDots(phaseIteration1, phaseIteration2) += patternArray(row, col);
                    } else if ((abs(std::fmod(phaseCol, 2 * PI)) >= PI - deltaPhase && abs(std::fmod(phaseCol, 2 * PI)) <= PI + deltaPhase) || (abs(std::fmod(phaseRow, 2 * PI)) >= PI - deltaPhase && abs(std::fmod(phaseRow, 2 * PI)) <= PI + deltaPhase)) {
                        numberBackgroundDots(phaseIteration1, phaseIteration2) += 1;
                        cumulBackgroundDots(phaseIteration1, phaseIteration2) += patternArray(row, col);
                    }
                }
            }
        }
    }


#ifdef USE_OPENCV 

    void Thumbnail::drawCodeDetection(cv::Mat& image) {
        cv::transpose(image, image);
        cv::Mat imageCode(image.rows + 200, image.cols + 200, CV_32FC1);
        image.convertTo(image, CV_32FC1);
        image.copyTo(imageCode(cv::Rect(0, 200, image.cols, image.rows)));

        image.convertTo(image, CV_32FC3);
        imageCode.convertTo(imageCode, CV_32FC3);
        cv::Mat imageCodeRGB(imageCode.rows, imageCode.cols, CV_32FC3);

        cv::cvtColor(imageCode, imageCodeRGB, cv::COLOR_GRAY2BGR);

        cv::namedWindow("coding Comparison", cv::WINDOW_FREERATIO);

        int period = 15;

        for (int i = 3; i < codeIntensity1.rows() - 3; i++) {
            cv::rectangle(imageCodeRGB, cv::Point(image.cols, (i - 3) * period + 200 - 2), cv::Point(image.cols + codeIntensity1(i, 0) * 150, (i - 3) * period + 10 + 200 + 2), cv::Scalar(1, 0, 0), cv::FILLED);
            cv::rectangle(imageCodeRGB, cv::Point(image.cols, (i - 3) * period + 2 + 200), cv::Point(image.cols + codeIntensity1(i, 2) * 150, (i - 3) * period + 8 + 200), cv::Scalar(0, 0, 1), cv::FILLED);
            cv::rectangle(imageCodeRGB, cv::Point(image.cols, (i - 3) * period + 1 + 200), cv::Point(image.cols + codeIntensity1(i, 1) * 150, (i - 3) * period + 9 + 200), cv::Scalar(0, 1, 0), cv::FILLED);
        }

        for (int i = 3; i < codeIntensity2.rows() - 3; i++) {
            cv::rectangle(imageCodeRGB, cv::Point((i - 3) * period - 2, 200), cv::Point((i - 3) * period + 10 + 2, 200 - codeIntensity2(i, 0) * 150), cv::Scalar(1, 0, 0), cv::FILLED);
            cv::rectangle(imageCodeRGB, cv::Point((i - 3) * period + 2, 200), cv::Point((i - 3) * period + 8, 200 - codeIntensity2(i, 2) * 150), cv::Scalar(0, 0, 1), cv::FILLED);
            cv::rectangle(imageCodeRGB, cv::Point((i - 3) * period + 1, 200), cv::Point((i - 3) * period + 9, 200 - codeIntensity2(i, 1) * 150), cv::Scalar(0, 1, 0), cv::FILLED);
        }

        cv::imshow("coding Comparison", imageCodeRGB);
    }
#endif // USE_OPENCV

    double Thumbnail::computeSTD2(Eigen::ArrayXXd image) {
        removeNanFromArray(image);
        double mean = image.mean();

        double sum = 0;

        for (int i = 0; i < image.rows(); i++) {
            for (int j = 0; j < image.cols(); j++) {
                sum += pow(image(i, j) - mean, 2);
            }
        }

        return sum / ((double) (image.rows() * image.cols()));
    }

#ifdef USE_OPENCV

    cv::Mat Thumbnail::getMeanDotsImage(int rectWidth) {
        //----------------------------------------------------------------------
        // feedback sur l'intensit� des points
        //----------------------------------------------------------------------
        Eigen::ArrayXXd meanWhiteDots = cumulWhiteDots / numberWhiteDots;
        Eigen::ArrayXXd meanBackgroundDots = cumulBackgroundDots / numberBackgroundDots;

        cv::Mat thumbnailImg(meanWhiteDots.cols() * rectWidth, meanWhiteDots.rows() * rectWidth, CV_64FC4);


        //        cv::Mat  thumbnailImg=array2image(cumulWhiteDots);
        //        cv::resize(thumbnailImg, thumbnailImg, cv::Size(meanWhiteDots.rows() * rectWidth, meanWhiteDots.cols() * rectWidth));

        for (int col = 0; col < meanWhiteDots.cols(); col++) {
            for (int row = 0; row < meanWhiteDots.rows(); row++) {
                int xPos = row * rectWidth; // transposition
                int yPos = col * rectWidth;
                double colorDot = meanWhiteDots(row, col);
                //                if (meanWhiteDots(row, col)>meanBackgroundDots(row, col)) {
                //                    colorDot = 1;
                //                } else {
                //                    colorDot = 0;
                //                }



                // double colorDot = numberWhiteDots(row, col)/(numberWhiteDots(row, col)+numberBackgroundDots(row, col));



                cv::Scalar cvColorDot = cv::Scalar(colorDot, colorDot, colorDot);
                cv::rectangle(thumbnailImg, cv::Point2d(xPos, yPos), cv::Point2d(xPos + rectWidth, yPos + rectWidth), cvColorDot, cv::FILLED);
            }
        }

        //cv::normalize(thumbnailImg, thumbnailImg, 1, 0, cv::NORM_MINMAX);

        return thumbnailImg;
    }

    void Thumbnail::guiMeanDots(cv::Mat& thumbnailImg) {
        //----------------------------------------------------------------------
        // feedback sur l'intensit� des points
        //----------------------------------------------------------------------
        Eigen::ArrayXXd meanWhiteDots = cumulWhiteDots / numberWhiteDots;
        //meanWhiteDots = meanWhiteDots/256;

        int rectWidth = 10;
        int depth = 1;

        cv::resize(thumbnailImg, thumbnailImg, cv::Size(meanWhiteDots.rows() * rectWidth, meanWhiteDots.cols() * rectWidth));

        for (int i = 0; i < meanWhiteDots.cols(); i++) {
            for (int j = 0; j < meanWhiteDots.rows(); j++) {
                int xPos = i * rectWidth;

                int yPos = j * rectWidth;

                double colorDot = meanWhiteDots(j, i);
                cv::Scalar cvColorDot = cv::Scalar(colorDot, colorDot, colorDot);
                cv::rectangle(thumbnailImg, cv::Point2d(xPos, yPos), cv::Point2d(xPos + rectWidth, yPos + rectWidth), cvColorDot / depth, cv::FILLED);
            }
        }

        cv::normalize(thumbnailImg, thumbnailImg, 1, 0, cv::NORM_MINMAX);

        //cv::imshow("thumbnail mean white dots", thumbnailImg);
        //---------------------------------------------------------------------
    }

    Eigen::ArrayXXd Thumbnail::guiMeanDots2() {
        //----------------------------------------------------------------------
        // feedback sur l'intensit� des points
        //----------------------------------------------------------------------
        Eigen::ArrayXXd meanWhiteDots = cumulWhiteDots / numberWhiteDots;
        //meanWhiteDots = meanWhiteDots/256;

        int rectWidth = 10;
        int depth = 1;

        cv::Mat thumbnailImg = cv::Mat(meanWhiteDots.rows() * rectWidth, meanWhiteDots.cols() * rectWidth, CV_64F);

        for (int i = 0; i < meanWhiteDots.cols(); i++) {
            for (int j = 0; j < meanWhiteDots.rows(); j++) {
                int xPos = i * rectWidth;

                int yPos = j * rectWidth;

                double colorDot = meanWhiteDots(j, i);
                cv::Scalar cvColorDot = cv::Scalar(colorDot, colorDot, colorDot);
                cv::rectangle(thumbnailImg, cv::Point2d(xPos, yPos), cv::Point2d(xPos + rectWidth, yPos + rectWidth), cvColorDot / depth, cv::FILLED);
            }
        }

        cv::normalize(thumbnailImg, thumbnailImg, 1, 0, cv::NORM_MINMAX);

        //cv::imshow("thumbnail mean white dots", thumbnailImg);
        Eigen::ArrayXXd imageArray;
        Eigen::MatrixXd imageMatrix;

        cv::cv2eigen(thumbnailImg, imageMatrix);
        imageArray = imageMatrix.array();

        return imageArray;
        //---------------------------------------------------------------------
    }

    void Thumbnail::drawThumbnailDetection(cv::Mat& thumbnailImg) {
        //----------------------------------------------------------------------
        // feedback sur l'intensit� des points
        //----------------------------------------------------------------------
        Eigen::ArrayXXd meanWhiteDots = cumulWhiteDots / numberWhiteDots;
        //meanWhiteDots = meanWhiteDots / 256;

        int rectWidth = 10;

        cv::resize(thumbnailImg, thumbnailImg, cv::Size(meanWhiteDots.rows() * rectWidth + 200, meanWhiteDots.cols() * rectWidth + 200));
        thumbnailImg.setTo(cv::Scalar(0, 0, 0));

        int depth = 4096;

        //cv::Mat thumbnailImg = cv::Mat(meanWhiteDots.rows() * rectWidth+200, meanWhiteDots.cols() * rectWidth+200, CV_64FC3);

        for (int i = 0; i < meanWhiteDots.cols(); i++) {
            for (int j = 0; j < meanWhiteDots.rows(); j++) {
                int xPos = i * rectWidth;

                int yPos = j * rectWidth;

                double colorDot = meanWhiteDots(j, i) / depth;
                cv::Scalar cvColorDot = cv::Scalar(colorDot, colorDot, colorDot);
                cv::rectangle(thumbnailImg, cv::Point2d(xPos, yPos + 200), cv::Point2d(xPos + rectWidth, yPos + rectWidth + 200), cvColorDot, cv::FILLED);
            }
        }

        cv::Mat text = cv::Mat(cv::Size(meanWhiteDots.rows() * rectWidth, 200), CV_64FC3);
        cv::Mat text2 = cv::Mat(cv::Size(200, meanWhiteDots.rows() * rectWidth), CV_64FC3);

        depth = 1;

        for (int i = 3; i < codeIntensity1.rows() - 3; i++) {
            if (codeIntensity1(i) < 4096) {
                cv::rectangle(thumbnailImg, cv::Point(thumbnailImg.cols - 200, 200 + (i - 0.75) * rectWidth), cv::Point(thumbnailImg.cols - 200 + (codeIntensity1(i, 2) / depth) * 150, 200 + (i + 1.75) * rectWidth), cv::Scalar(0, 1, 1), cv::FILLED);
                cv::rectangle(thumbnailImg, cv::Point(thumbnailImg.cols - 200, 200 + (i - 0.75) * rectWidth), cv::Point(thumbnailImg.cols - 200 + (codeIntensity1(i, 2) / depth) * 150, 200 + (i + 1.75) * rectWidth), cv::Scalar(1, 1, 1), 1);
                cv::rectangle(thumbnailImg, cv::Point(thumbnailImg.cols - 200, 200 + (i - 0.75) * rectWidth), cv::Point(thumbnailImg.cols - 200 + (codeIntensity1(i, 1) / depth) * 150, 200 + (i + 1.75) * rectWidth), cv::Scalar(0.25, 0.25, 0.25), cv::FILLED);
                cv::rectangle(thumbnailImg, cv::Point(thumbnailImg.cols - 200, 200 + (i - 0.75) * rectWidth), cv::Point(thumbnailImg.cols - 200 + (codeIntensity1(i, 1) / depth) * 150, 200 + (i + 1.75) * rectWidth), cv::Scalar(1, 1, 1), 1);
                cv::rectangle(thumbnailImg, cv::Point(thumbnailImg.cols - 200, 200 + i * rectWidth), cv::Point(thumbnailImg.cols - 200 + (codeIntensity1(i, 0) / depth) * 150, 200 + (i + 1) * rectWidth), cv::Scalar(1, 0, 1), cv::FILLED);
                if (sequence1(i) != 0) {
                    //if (trueSequence1(i / 3) == 0) {
                    //	//cv::putText(text, "0", cv::Point(i * rectWidth - 5, 50), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 1, 0), 2);
                    //	if (trueSequence1(i / 3) == sequence1(i) + 1) {
                    //		cv::putText(text, "0", cv::Point(i * rectWidth - 5, 70), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 1, 0), 2);
                    //	}
                    //	else {
                    //		cv::putText(text, std::to_string((int)(sequence1(i))), cv::Point(i * rectWidth - 5, 70), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 1), 2);
                    //	}
                    //}
                    //else {
                    //	//cv::putText(text, "1", cv::Point(i * rectWidth - 5, 50), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 1, 0), 2);
                    //	if (trueSequence1(i / 3) == sequence1(i)) {
                    //		cv::putText(text, std::to_string((int)(sequence1(i))), cv::Point(i * rectWidth - 5, 70), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 1, 0), 2);
                    //	}
                    //	else {
                    //		cv::putText(text, "0", cv::Point(i * rectWidth - 5, 70), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 1), 2);
                    //	}
                    //}

                    if (sequence1(i) == 1) {
                        cv::putText(text, std::to_string((int) (sequence1(i))), cv::Point(i * rectWidth - 5, 50), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(1, 1, 1), 2);
                    } else {
                        cv::putText(text, "0", cv::Point(i * rectWidth - 5, 50), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(1, 1, 1), 2);
                    }
                }
            }
        }
        for (int i = 1; i < codeIntensity2.rows() - 2; i++) {
            if (codeIntensity2(i) < 4096) {
                cv::rectangle(thumbnailImg, cv::Point((i - 0.75) * rectWidth, 200), cv::Point((i + 1.75) * rectWidth, 200 - (codeIntensity2(i, 2) / depth) * 150), cv::Scalar(0, 1, 1), cv::FILLED);
                cv::rectangle(thumbnailImg, cv::Point((i - 0.75) * rectWidth, 200), cv::Point((i + 1.75) * rectWidth, 200 - (codeIntensity2(i, 2) / depth) * 150), cv::Scalar(1, 1, 1), 1);
                cv::rectangle(thumbnailImg, cv::Point((i - 0.75) * rectWidth, 200), cv::Point((i + 1.75) * rectWidth, 200 - (codeIntensity2(i, 1) / depth) * 150), cv::Scalar(0.25, 0.25, 0.25), cv::FILLED);
                cv::rectangle(thumbnailImg, cv::Point((i - 0.75) * rectWidth, 200), cv::Point((i + 1.75) * rectWidth, 200 - (codeIntensity2(i, 1) / depth) * 150), cv::Scalar(1, 1, 1), 1);
                cv::rectangle(thumbnailImg, cv::Point(i * rectWidth, 200), cv::Point((i + 1) * rectWidth, 200 - (codeIntensity2(i, 0) / depth) * 150), cv::Scalar(1, 1, 0), cv::FILLED);
                if (sequence2(i) != 0) {
                    //if (trueSequence2(i / 3) == 0) {
                    //	//cv::putText(thumbnailImg, "0", cv::Point(i * rectWidth - 5, 50), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 1, 0), 2);
                    //	if (trueSequence2(i / 3) == sequence2(i) + 1) {
                    //		cv::putText(thumbnailImg, "0", cv::Point(i * rectWidth - 5, 70), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 1, 0), 2);
                    //	}
                    //	else {
                    //		cv::putText(thumbnailImg, std::to_string((int)(sequence2(i))), cv::Point(i * rectWidth - 5, 70), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 1), 2);
                    //	}
                    //}
                    //else {
                    //	//cv::putText(thumbnailImg, "1", cv::Point(i * rectWidth - 5, 50), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 1, 0), 2);
                    //	if (trueSequence2(i / 3) == sequence2(i)) {
                    //		cv::putText(thumbnailImg, std::to_string((int)(sequence2(i))), cv::Point(i * rectWidth - 5, 70), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 1, 0), 2);
                    //	}
                    //	else {
                    //		cv::putText(thumbnailImg, "0", cv::Point(i * rectWidth - 5, 70), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 1), 2);
                    //	}
                    //}

                    if (sequence2(i) == 1) {
                        cv::putText(thumbnailImg, std::to_string((int) (sequence2(i))), cv::Point(i * rectWidth - 5, 50), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(1, 1, 1), 2);
                    } else {
                        cv::putText(thumbnailImg, "0", cv::Point(i * rectWidth - 5, 50), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(1, 1, 1), 2);
                    }
                }
            }
        }

        cv::Mat M = cv::getRotationMatrix2D(cv::Point(text.rows / 2, text.rows / 2), -90, 1);
        //cv::Mat M2 = (cv::Mat_<double>(2, 3) << 1, 0, 67.5, 0, 1, -200);
        cv::warpAffine(text, text2, M, cv::Size(text2.cols, text2.rows));
        //cv::warpAffine(text, text, M2, cv::Size(text.cols, text.rows));

        thumbnailImg(cv::Rect(thumbnailImg.cols - text2.cols, 200, text2.cols, text2.rows)) += text2;

        //text2.copyTo(thumbnailImg(cv::Rect(thumbnailImg.cols - text2.cols, 200,text2.cols,text2.rows)));

        //cv::normalize(thumbnailImg, thumbnailImg, 1, 0, cv::NORM_MINMAX);

        //---------------------------------------------------------------------
    }

    void Thumbnail::drawSequence1Level(Eigen::ArrayXXd& codingLevelSecurity1, Eigen::ArrayXXd& codingLevelSecurity2) {
        int rectWidth = 20;
        Eigen::ArrayXXd meanWhiteDots = cumulWhiteDots / numberWhiteDots;
        Eigen::ArrayXXd meanBackDots = cumulBackgroundDots / numberBackgroundDots;

        for (int i = 0; i < meanWhiteDots.rows(); i++) {
            for (int j = 0; j < meanWhiteDots.cols(); j++) {
                if (isnan(meanWhiteDots(i, j))) {
                    meanWhiteDots(i, j) = 0;
                }
                if (isnan(meanBackDots(i, j))) {
                    meanBackDots(i, j) = 0;
                }
            }
        }

        cv::Mat sequence1Mat = cv::Mat(300, rectWidth * codeIntensity2.rows(), CV_64FC3);

        for (int i = 5; i < codeIntensity2.rows() - 5; i++) {
            if (codeIntensity2(i) < 200) {
                cv::rectangle(sequence1Mat, cv::Point((i - 0.75) * rectWidth, 300), cv::Point((i + 1.75) * rectWidth, 300 - codeIntensity2(i, 2) * 300), cv::Scalar(0, 1, 1), cv::FILLED);
                cv::rectangle(sequence1Mat, cv::Point((i - 0.75) * rectWidth, 300), cv::Point((i + 1.75) * rectWidth, 300 - codeIntensity2(i, 1) * 300), cv::Scalar(0, 1, 0), cv::FILLED);
                cv::rectangle(sequence1Mat, cv::Point(i * rectWidth, 300), cv::Point((i + 1) * rectWidth, 300 - codeIntensity2(i, 0) * 300), cv::Scalar(1, 1, 0), cv::FILLED);

                if (sequence2(i) != 0) {
                    if (sequence2(i) == 1) {
                        cv::putText(sequence1Mat, "1", cv::Point(i * rectWidth + 5, 60), cv::FONT_HERSHEY_COMPLEX, 0.75, cv::Scalar(1, 1, 1), 2);
                    } else {
                        cv::putText(sequence1Mat, "0", cv::Point(i * rectWidth + 5, 60), cv::FONT_HERSHEY_COMPLEX, 0.75, cv::Scalar(1, 1, 1), 2);
                    }

                    double codingLevel1 = (codeIntensity2(i, 0) - codeIntensity2(i, 1)) / (codeIntensity2(i, 2) - codeIntensity2(i, 1));

                    double contrastLevel = (codeIntensity2(i, 2) - codeIntensity2(i, 1));

                    codingLevelSecurity1.conservativeResize(codingLevelSecurity1.rows() + 1, codingLevelSecurity1.cols());

                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 0) = contrastLevel;
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 1) = codingLevel1;
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 2) = numberWhiteRefDots1(i - 1);
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 3) = numberBackRefDots1(i - 1);
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 4) = numberCodingDots1(i - 1);
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 5) = codeIntensity2(i, 0);
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 6) = codeIntensity2(i, 1);
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 7) = codeIntensity2(i, 2);
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 8) = sequence2(i);
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 9) = i;
                    codingLevelSecurity1(codingLevelSecurity1.rows() - 1, 10) = sequence2.rows();
                }
            }
        }

        cv::putText(sequence1Mat, "sequence 1", cv::Point(20, 20), cv::FONT_HERSHEY_COMPLEX, 0.75, cv::Scalar(1, 1, 1), 1);
        cv::imshow("sequence1levels", sequence1Mat);

        cv::Mat sequence2Mat = cv::Mat(300, rectWidth * codeIntensity1.rows(), CV_64FC3);

        for (int i = 5; i < codeIntensity1.rows() - 5; i++) {
            if (codeIntensity1(i) < 200) {
                cv::rectangle(sequence2Mat, cv::Point((i - 0.75) * rectWidth, 300), cv::Point((i + 1.75) * rectWidth, 300 - codeIntensity1(i, 2) * 300), cv::Scalar(0, 1, 1), cv::FILLED);
                cv::rectangle(sequence2Mat, cv::Point((i - 0.75) * rectWidth, 300), cv::Point((i + 1.75) * rectWidth, 300 - codeIntensity1(i, 1) * 300), cv::Scalar(0, 1, 0), cv::FILLED);
                cv::rectangle(sequence2Mat, cv::Point(i * rectWidth, 300), cv::Point((i + 1) * rectWidth, 300 - codeIntensity1(i, 0) * 300), cv::Scalar(1, 0, 1), cv::FILLED);

                if (sequence1(i) != 0) {
                    if (sequence1(i) == 1) {
                        cv::putText(sequence2Mat, "1", cv::Point(i * rectWidth + 5, 60), cv::FONT_HERSHEY_COMPLEX, 0.75, cv::Scalar(1, 1, 1), 2);
                    } else {
                        cv::putText(sequence2Mat, "0", cv::Point(i * rectWidth + 5, 60), cv::FONT_HERSHEY_COMPLEX, 0.75, cv::Scalar(1, 1, 1), 2);
                    }
                    double codingLevel2 = (codeIntensity1(i, 0) - codeIntensity1(i, 1)) / (codeIntensity1(i, 2) - codeIntensity1(i, 1));

                    codingLevelSecurity2.conservativeResize(codingLevelSecurity2.rows() + 1, codingLevelSecurity2.cols());

                    double contrastLevel = (codeIntensity1(i, 2) - codeIntensity1(i, 1));
                    //std::cout << codeIntensity1(i, 2) << "     " << codeIntensity1(i, 1) << std::endl;

                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 0) = contrastLevel;
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 1) = codingLevel2;
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 2) = numberWhiteRefDots2(i - 2);
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 3) = numberBackRefDots2(i - 2);
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 4) = numberCodingDots2(i - 2);
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 5) = codeIntensity1(i, 0);
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 6) = codeIntensity1(i, 1);
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 7) = codeIntensity1(i, 2);
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 8) = sequence1(i);
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 9) = i;
                    codingLevelSecurity2(codingLevelSecurity2.rows() - 1, 10) = sequence1.rows();

                    //std::cout << numberWhiteRefDots2(i - 2) << std::endl;
                }
            }
        }
        cv::putText(sequence2Mat, "sequence 2", cv::Point(20, 20), cv::FONT_HERSHEY_COMPLEX, 0.75, cv::Scalar(1, 1, 1), 1);
        cv::imshow("sequence2levels", sequence2Mat);
    }

    void Thumbnail::guiSequences(cv::Mat& sequencesBiDir) {
        double margin = 1;
        double marge = 1;
        int rectWidth = 5;

        cv::resize(sequencesBiDir, sequencesBiDir, cv::Size(sequence2.rows() * (rectWidth + marge), sequence1.rows() * (rectWidth + marge)));

        for (int i = 0; i < sequence1.rows(); i++) {
            for (int j = 0; j < sequence2.rows(); j++) {
                int xPos = i * (rectWidth + margin);

                int yPos = j * (rectWidth + margin);

                if (sequence1(i) == 0 && sequence2(j) == 0) {
                    cv::rectangle(sequencesBiDir, cv::Point2d(xPos + marge, yPos + marge), cv::Point2d(xPos + rectWidth - marge, yPos + rectWidth - marge), cv::Scalar(1, 1, 1), cv::FILLED);
                }

                if (sequence1(i) == 1 && sequence2(j) == 1) {
                    cv::rectangle(sequencesBiDir, cv::Point2d(xPos + marge, yPos + marge), cv::Point2d(xPos + rectWidth - marge, yPos + rectWidth - marge), cv::Scalar(1, 1, 1), cv::FILLED);
                }

                if (sequence1(i) == 1 && sequence2(j) == 0) {
                    cv::rectangle(sequencesBiDir, cv::Point2d(xPos + marge, yPos + marge), cv::Point2d(xPos + rectWidth - marge, yPos + rectWidth - marge), cv::Scalar(1, 1, 1), cv::FILLED);
                }

                if (sequence1(i) == 0 && sequence2(j) == 1) {
                    cv::rectangle(sequencesBiDir, cv::Point2d(xPos + marge, yPos + marge), cv::Point2d(xPos + rectWidth - marge, yPos + rectWidth - marge), cv::Scalar(1, 1, 1), cv::FILLED);
                }

                if (i % 3 == codeOrientation(2) && j % 3 == codeOrientation(3)) {
                    cv::rectangle(sequencesBiDir, cv::Point2d(xPos + marge, yPos + marge), cv::Point2d(xPos + rectWidth - marge, yPos + rectWidth - marge), cv::Scalar(0, 0, 0), cv::FILLED);
                }

                if (i % 3 == 0) {
                    cv::line(sequencesBiDir, cv::Point(xPos + marge / 2, 0), cv::Point(xPos + marge / 2, sequence1.rows() * (rectWidth + marge)), cv::Scalar(0, 0, 255), 1);
                }

                if (j % 3 == 0) {
                    cv::line(sequencesBiDir, cv::Point(0, yPos + marge / 2), cv::Point(sequence1.rows() * (rectWidth + marge), yPos + marge / 2), cv::Scalar(0, 0, 255), 1);
                }
            }
        }

        cv::imshow("sequence bi dir thumbnail", sequencesBiDir);
    }

    void Thumbnail::guiDisplayCode() {
        int coding1 = codeOrientation(0);
        int coding2 = codeOrientation(1);
        int missing1 = codeOrientation(2);
        int missing2 = codeOrientation(3);

        cv::Mat codeOrientationImg = cv::Mat(300, 300, CV_64FC3);
        cv::rectangle(codeOrientationImg, cv::Point(0, 0), cv::Point(300, 300), cv::Scalar(1, 0, 0), cv::FILLED);
        cv::rectangle(codeOrientationImg, cv::Point(coding1 * 100, 0), cv::Point(coding1 * 100 + 100, 300), cv::Scalar(0, 1, 0), cv::FILLED);
        cv::rectangle(codeOrientationImg, cv::Point(0, coding2 * 100), cv::Point(300, coding2 * 100 + 100), cv::Scalar(0, 1, 0), cv::FILLED);
        cv::rectangle(codeOrientationImg, cv::Point(missing1 * 100, missing2 * 100), cv::Point(missing1 * 100 + 100, missing2 * 100 + 100), cv::Scalar(0, 0, 0), cv::FILLED);
        cv::imshow("coding cell", codeOrientationImg);
    }

    void Thumbnail::guiCodeDirection() {
        cv::Mat codeDirection = cv::Mat(300, 300, CV_64FC3);
        if (MSB1 == 1 && MSB2 == 1) {
            cv::line(codeDirection, cv::Point(150, 150), cv::Point(50, 50), cv::Scalar(0, 255, 0), 4);
            cv::line(codeDirection, cv::Point(50, 50), cv::Point(55, 50), cv::Scalar(0, 255, 0), 4);
            cv::line(codeDirection, cv::Point(50, 50), cv::Point(50, 55), cv::Scalar(0, 255, 0), 4);
        }
        if (MSB1 == 0 && MSB2 == 1) {
            cv::line(codeDirection, cv::Point(150, 150), cv::Point(250, 50), cv::Scalar(255, 0, 0), 4);
            cv::line(codeDirection, cv::Point(250, 50), cv::Point(245, 50), cv::Scalar(255, 0, 0), 4);
            cv::line(codeDirection, cv::Point(250, 50), cv::Point(250, 55), cv::Scalar(255, 0, 0), 4);
        }
        if (MSB1 == 0 && MSB2 == 0) {
            cv::line(codeDirection, cv::Point(150, 150), cv::Point(250, 250), cv::Scalar(0, 0, 255), 4);
            cv::line(codeDirection, cv::Point(250, 250), cv::Point(245, 250), cv::Scalar(0, 0, 255), 4);
            cv::line(codeDirection, cv::Point(250, 250), cv::Point(250, 245), cv::Scalar(0, 0, 255), 4);
        }
        if (MSB1 == 1 && MSB2 == 0) {
            cv::line(codeDirection, cv::Point(150, 150), cv::Point(50, 250), cv::Scalar(0, 128, 128), 4);
            cv::line(codeDirection, cv::Point(50, 250), cv::Point(55, 250), cv::Scalar(0, 128, 128), 4);
            cv::line(codeDirection, cv::Point(50, 250), cv::Point(50, 245), cv::Scalar(0, 128, 128), 4);
        }
        cv::imshow("code direction", codeDirection);
    }
#endif // USE_OPENCV

    Eigen::ArrayXXd Thumbnail::getThumbnailArray() {
        Eigen::ArrayXXd meanWhiteDots = cumulWhiteDots / numberWhiteDots;
        return meanWhiteDots;
    }

    Eigen::ArrayXXd Thumbnail::getBackgroundForegroundArray() {
        Eigen::ArrayXXd meanWhiteDots = cumulWhiteDots / numberWhiteDots;
        Eigen::ArrayXXd meanBackDots = cumulBackgroundDots / numberBackgroundDots;

        Eigen::ArrayXXd fullThumbnail(2 * meanBackDots.rows() + 1, 2 * meanBackDots.cols() + 1);

        for (int row = 0; row < meanWhiteDots.rows() - 1; row++) {
            for (int col = 0; col < meanWhiteDots.cols() - 1; col++) {
                fullThumbnail(2 * row, 2 * col) = (meanBackDots(row, col) / meanWhiteDots(row, col));
                fullThumbnail(2 * row + 1, 2 * col) = ((meanBackDots(row, col) / meanWhiteDots(row, col)) + (meanBackDots(row + 1, col) / meanWhiteDots(row + 1, col))) / 2;
                fullThumbnail(2 * row, 2 * col + 1) = ((meanBackDots(row, col) / meanWhiteDots(row, col)) + (meanBackDots(row, col + 1) / meanWhiteDots(row, col + 1))) / 2;
                fullThumbnail(2 * row + 1, 2 * col + 1) = ((meanBackDots(row, col) / meanWhiteDots(row, col)) + (meanBackDots(row, col + 1) / meanWhiteDots(row, col + 1)) + (meanBackDots(row + 1, col) / meanWhiteDots(row + 1, col)) + (meanBackDots(row + 1, col + 1) / meanWhiteDots(row + 1, col + 1))) / 4;
                //fullThumbnail(2 * row + 1, 2 * col + 1) = meanBackDots(row, col);
                //fullThumbnail(2 * row, 2 * col + 1) = meanBackDots(row, col);
                //fullThumbnail(2 * row + 1, 2 * col) = meanBackDots(row, col);
            }
        }

        return fullThumbnail;
    }

    Eigen::VectorXd Thumbnail::getSequence1() {
        return sequence1;
    }

    Eigen::VectorXd Thumbnail::getSequence2() {
        return sequence2;
    }

    Eigen::VectorXd Thumbnail::getCodeOrientation() {
        return codeOrientation;
    }

    int Thumbnail::getMSB1() {
        return MSB1;
    }

    int Thumbnail::getMSB2() {
        return MSB2;
    }

    Eigen::ArrayXXd Thumbnail::getNumberWhiteDots() {
        return numberWhiteDots;
    }

    Eigen::ArrayXXd Thumbnail::getCumulWhiteDots() {
        return cumulWhiteDots;
    }

    Eigen::ArrayXXd Thumbnail::getNumberBackground() {
        return numberBackgroundDots;
    }

    Eigen::ArrayXXd Thumbnail::getCumulBackground() {
        return cumulBackgroundDots;
    }

    void Thumbnail::removeNanFromArray(Eigen::ArrayXXd& image) {
        for (int i = 0; i < image.rows(); i++) {
            for (int j = 0; j < image.cols(); j++) {
                if (isnan(image(i, j))) {
                    image(i, j) = 0;
                }
            }
        }
    }
}