/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "MegarenaCell.hpp"

namespace vernier {

    MegarenaCell::MegarenaCell() {
    }

    void MegarenaCell::resize() {
    }

    void MegarenaCell::getGlobalCell(Eigen::ArrayXXd thumbnailNumberWhiteDots, Eigen::ArrayXXd thumbnailCumulWhiteDots) {
        Eigen::Array33d sumOnlyDotsRemain = Eigen::Array33d::Zero(3, 3);
        Eigen::Array33d countOnlyDots = Eigen::Array33d::Zero(3, 3);

        for (int col = 0; col < thumbnailNumberWhiteDots.cols(); col++) {
            for (int row = 0; row < thumbnailNumberWhiteDots.rows(); row++) {
                int index1 = row % 3;
                int index2 = col % 3;

                sumOnlyDotsRemain(index1, index2) += thumbnailCumulWhiteDots(row, col);
                countOnlyDots(index1, index2) += thumbnailNumberWhiteDots(row, col);
            }
        }

        sumOnlyDotsRemain = sumOnlyDotsRemain / countOnlyDots;

        this->globalCell = sumOnlyDotsRemain;
    }

    Eigen::VectorXd MegarenaCell::getCodeOrientation() {
        Eigen::Matrix<Eigen::Array33d, 3, 3> orientation1;
        Eigen::Matrix<Eigen::Array33d, 3, 3> orientation2;
        Eigen::Matrix<Eigen::Array33d, 3, 3> orientation3;
        Eigen::Matrix<Eigen::Array33d, 3, 3> orientation4;

        // Orientation 1
        orientation1(0, 0) << 0, 0, 0,
                0, 1, 1,
                0, 1, -1;

        orientation1(0, 1) << 0, 1, 1,
                0, 0, 0,
                0, 1, -1;

        orientation1(0, 2) << 0, 1, 1,
                0, 1, -1,
                0, 0, 0;

        orientation1(1, 0) << 0, 0, 0,
                1, 0, 1,
                1, 0, -1;

        orientation1(1, 1) << 1, 0, 1,
                0, 0, 0,
                1, 0, -1;

        orientation1(1, 2) << 1, 0, 1,
                1, 0, -1,
                0, 0, 0;

        orientation1(2, 0) << 0, 0, 0,
                1, 1, 0,
                1, -1, 0;

        orientation1(2, 1) << 1, 1, 0,
                0, 0, 0,
                1, -1, 0;

        orientation1(2, 2) << 1, 1, 0,
                1, -1, 0,
                0, 0, 0;

        // Orientation 2
        orientation2(0, 0) << 0, 0, 0,
                0, 1, 1,
                0, -1, 1;

        orientation2(0, 1) << 0, 1, 1,
                0, 0, 0,
                0, -1, 1;

        orientation2(0, 2) << 0, 1, 1,
                0, -1, 1,
                0, 0, 0;

        orientation2(1, 0) << 0, 0, 0,
                1, 0, 1,
                -1, 0, 1;

        orientation2(1, 1) << 1, 0, 1,
                0, 0, 0,
                -1, 0, 1;

        orientation2(1, 2) << 1, 0, 1,
                -1, 0, 1,
                0, 0, 0;

        orientation2(2, 0) << 0, 0, 0,
                1, 1, 0,
                -1, 1, 0;

        orientation2(2, 1) << 1, 1, 0,
                0, 0, 0,
                -1, 1, 0;

        orientation2(2, 2) << 1, 1, 0,
                -1, 1, 0,
                0, 0, 0;

        // Orientatin 3
        orientation3(0, 0) << 0, 0, 0,
                0, 1, -1,
                0, 1, 1;

        orientation3(0, 1) << 0, 1, -1,
                0, 0, 0,
                0, 1, 1;

        orientation3(0, 2) << 0, 1, -1,
                0, 1, 1,
                0, 0, 0;

        orientation3(1, 0) << 0, 0, 0,
                1, 0, -1,
                1, 0, 1;

        orientation3(1, 1) << 1, 0, -1,
                0, 0, 0,
                1, 0, 1;

        orientation3(1, 2) << 1, 0, -1,
                1, 0, 1,
                0, 0, 0;

        orientation3(2, 0) << 0, 0, 0,
                1, -1, 0,
                1, 1, 0;

        orientation3(2, 1) << 1, -1, 0,
                0, 0, 0,
                1, 1, 0;

        orientation3(2, 2) << 1, -1, 0,
                1, 1, 0,
                0, 0, 0;

        // Orientation 4
        orientation4(0, 0) << 0, 0, 0,
                0, -1, 1,
                0, 1, 1;

        orientation4(0, 1) << 0, -1, 1,
                0, 0, 0,
                0, 1, 1;

        orientation4(0, 2) << 0, -1, 1,
                0, 1, 1,
                0, 0, 0;

        orientation4(1, 0) << 0, 0, 0,
                -1, 0, 1,
                1, 0, 1;

        orientation4(1, 1) << -1, 0, 1,
                0, 0, 0,
                1, 0, 1;

        orientation4(1, 2) << -1, 0, 1,
                1, 0, 1,
                0, 0, 0;

        orientation4(2, 0) << 0, 0, 0,
                -1, 1, 0,
                1, 1, 0;

        orientation4(2, 1) << -1, 1, 0,
                0, 0, 0,
                1, 1, 0;

        orientation4(2, 2) << -1, 1, 0,
                1, 1, 0,
                0, 0, 0;

        double maxi = 0;
        int coding1 = 0;
        int coding2 = 0;
        int quadrant = 0;

        Eigen::Array33d totalSum1;
        Eigen::Array33d totalSum2;
        Eigen::Array33d totalSum3;
        Eigen::Array33d totalSum4;

        Eigen::Vector4d sumVector;

        for (int index1 = 0; index1 < 3; index1++) {
            for (int index2 = 0; index2 < 3; index2++) {
                totalSum1 = globalCell * orientation1(index2, index1);

                totalSum2 = globalCell * orientation2(index2, index1);

                totalSum3 = globalCell * orientation3(index2, index1);

                totalSum4 = globalCell * orientation4(index2, index1);

                sumVector << totalSum1.sum(), totalSum2.sum(), totalSum3.sum(), totalSum4.sum();

                for (int indexOrientation = 0; indexOrientation < 4; indexOrientation++) {
                    if (sumVector(indexOrientation) > maxi) {
                        maxi = sumVector(indexOrientation);
                        coding1 = index1;
                        coding2 = index2;
                        quadrant = indexOrientation;
                    }
                }
            }
        }

        int missing1, missing2;

        if (quadrant == 0 || quadrant == 1) {
            if (coding1 == 2) {
                missing1 = 1;
            } else {
                missing1 = 2;
            }
        }
        if (quadrant == 2 || quadrant == 3) {
            if (coding1 == 0) {
                missing1 = 1;
            } else {
                missing1 = 0;
            }
        }
        if (quadrant == 0 || quadrant == 2) {
            if (coding2 == 2) {
                missing2 = 1;
            } else {
                missing2 = 2;
            }
        }
        if (quadrant == 1 || quadrant == 3) {
            if (coding2 == 0) {
                missing2 = 1;
            } else {
                missing2 = 0;
            }
        }

        Eigen::VectorXd codingLines(5);
        codingLines << coding1, coding2, missing1, missing2, quadrant;
        this->codeOrientation = codingLines;

        //guiDisplayCell();

        return codingLines;
    }

    Eigen::Array33d MegarenaCell::getGlobalCellOut() {
        return globalCell;
    }

    void MegarenaCell::guiDisplayCell(cv::Mat& codingCell) {
        int depth = 1;
        int coding1 = codeOrientation(0);
        int coding2 = codeOrientation(1);
        int missing1 = codeOrientation(2);
        int missing2 = codeOrientation(3);
        cv::resize(codingCell, codingCell, cv::Size(700, 180));
        codingCell.setTo(cv::Scalar(0, 0, 0));

        int nb_affichageMAX = 0;
        int nb_affichageCoding1 = 0;
        int nb_affichageCoding2 = 0;
        int nb_affichageCombined = 0;

        globalCell = globalCell / globalCell.maxCoeff();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i == coding1 && j != coding2) {
                    cv::rectangle(codingCell, cv::Point(i * 60, j * 60), cv::Point((i + 1) * 60, (j + 1) * 60), cv::Scalar(1, 1, 0), cv::FILLED);
                    cv::putText(codingCell, "d1", cv::Point(i * 60 + 25, j * 60 + 35), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 0), 1);

                    if (nb_affichageCoding1 == 0 && (int) (globalCell(i, j) * 4096) != 0) {
                        nb_affichageCoding1++;
                        cv::rectangle(codingCell, cv::Point(340, 180), cv::Point(400, 180 - 180 * globalCell(i, j) / depth), cv::Scalar(1, 1, 0), cv::FILLED);
                        cv::putText(codingCell, to_string((int) (globalCell(i, j) * 4096)), cv::Point(350, 180 - (180 * globalCell(i, j) / depth - 30)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                        cv::putText(codingCell, "d1 ", cv::Point(350, 180 - (180 * globalCell(i, j) / depth - 15)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                    }

                    if (nb_affichageCoding1 == 1 && (int) (globalCell(i, j) * 4096) != 0) {
                        nb_affichageCoding1++;
                        cv::rectangle(codingCell, cv::Point(410, 180), cv::Point(470, 180 - 180 * globalCell(i, j) / depth), cv::Scalar(1, 1, 0), cv::FILLED);
                        cv::putText(codingCell, to_string((int) (globalCell(i, j) * 4096)), cv::Point(420, 180 - (180 * globalCell(i, j) / depth - 30)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                        cv::putText(codingCell, "d1 ", cv::Point(420, 180 - (180 * globalCell(i, j) / depth - 15)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                    }
                }

                if (i != coding1 && j == coding2) {
                    cv::rectangle(codingCell, cv::Point(i * 60, j * 60), cv::Point((i + 1) * 60, (j + 1) * 60), cv::Scalar(1, 0, 1), cv::FILLED);
                    cv::putText(codingCell, "d2", cv::Point(i * 60 + 25, j * 60 + 35), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 0), 1);

                    if (nb_affichageCoding2 == 0 && (int) (globalCell(i, j) * 4096) != 0) {
                        nb_affichageCoding2++;
                        cv::rectangle(codingCell, cv::Point(480, 180), cv::Point(540, 180 - 180 * globalCell(i, j) / depth), cv::Scalar(1, 0, 1), cv::FILLED);
                        cv::putText(codingCell, std::to_string((int) (globalCell(i, j) * 4096)), cv::Point(490, 180 - (180 * globalCell(i, j) / depth - 30)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                        cv::putText(codingCell, "d2 ", cv::Point(490, 180 - (180 * globalCell(i, j) / depth - 15)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                    }
                    if (nb_affichageCoding2 == 1 && (int) (globalCell(i, j) * 4096) != 0) {
                        nb_affichageCoding2++;
                        cv::rectangle(codingCell, cv::Point(550, 180), cv::Point(610, 180 - 180 * globalCell(i, j) / depth), cv::Scalar(1, 0, 1), cv::FILLED);
                        cv::putText(codingCell, to_string((int) (globalCell(i, j) * 4096)), cv::Point(560, 180 - (180 * globalCell(i, j) / depth - 30)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                        cv::putText(codingCell, "d2 ", cv::Point(560, 180 - (180 * globalCell(i, j) / depth - 15)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                    }
                }

                if (i == coding1 && j == coding2) {
                    cv::rectangle(codingCell, cv::Point(i * 60, j * 60), cv::Point((i + 1) * 60, (j + 1) * 60), cv::Scalar(0.33, 0, 0.33), cv::FILLED);
                    cv::putText(codingCell, "d1.d2", cv::Point(i * 60 + 5, j * 60 + 35), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.7, cv::Scalar(1, 1, 1), 1);

                    cv::rectangle(codingCell, cv::Point(270, 180), cv::Point(330, 180 - 180 * globalCell(i, j) / depth), cv::Scalar(0.33, 0, 0.33), cv::FILLED);
                    if (nb_affichageCombined < 1 && (int) (globalCell(i, j) * 4096) != 0) {
                        nb_affichageCombined++;
                        cv::putText(codingCell, to_string((int) (globalCell(i, j) * 4096)), cv::Point(280, 180 - (180 * globalCell(i, j) / depth - 30)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(1, 1, 1), 1);
                        cv::putText(codingCell, "d1.d2 ", cv::Point(280, 180 - (180 * globalCell(i, j) / depth - 15)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(1, 1, 1), 1);
                    }
                }

                if (i != coding1 && j != coding2) {
                    if (i == missing1 && j == missing2) {
                        cv::rectangle(codingCell, cv::Point(i * 60, j * 60), cv::Point((i + 1) * 60, (j + 1) * 60), cv::Scalar(0.2, 0.2, 0.2), cv::FILLED);
                        cv::putText(codingCell, "min", cv::Point(i * 60 + 5, j * 60 + 35), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(1, 1, 1), 1);

                        cv::rectangle(codingCell, cv::Point(200, 180), cv::Point(260, 180 - (180 * globalCell(i, j) / depth + 5)), cv::Scalar(0.2, 0.2, 0.2), cv::FILLED);
                        cv::rectangle(codingCell, cv::Point(200, 180), cv::Point(260, 180 - (180 * globalCell(i, j) / depth + 5)), cv::Scalar(1, 1, 1), 1);
                        cv::putText(codingCell, to_string((int) (globalCell(i, j) * 4096)), cv::Point(210, 180 - (180 * globalCell(i, j) / depth + 10)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(1, 1, 1), 1);
                        cv::putText(codingCell, "min ", cv::Point(210, 180 - (180 * globalCell(i, j) / depth + 25)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(1, 1, 1), 1);
                    } else {
                        cv::rectangle(codingCell, cv::Point(i * 60, j * 60), cv::Point((i + 1) * 60, (j + 1) * 60), cv::Scalar(0, 1, 1), cv::FILLED);
                        cv::putText(codingCell, "max", cv::Point(i * 60 + 5, j * 60 + 35), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 0), 1);

                        if (nb_affichageMAX < 1 && (int) (globalCell(i, j) * 4096) != 0) {
                            cv::rectangle(codingCell, cv::Point(620, 180), cv::Point(680, 180 - 180 * globalCell(i, j) / depth), cv::Scalar(0, 1, 1), cv::FILLED);

                            nb_affichageMAX++;
                            cv::putText(codingCell, to_string((int) (globalCell(i, j) * 4096)), cv::Point(630, 180 - (180 * globalCell(i, j) / depth - 25)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                            cv::putText(codingCell, "max ", cv::Point(630, 180 - (180 * globalCell(i, j) / depth - 10)), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
                        }
                    }
                }
            }
        }
    }

}