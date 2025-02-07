/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */


#include "Cell.hpp"
#include "UnitTest.hpp"
#include "eigen-matio/MatioFile.hpp"

using namespace vernier;

void main1() {
    Eigen::MatioFile file("data/TestFilesMatMegarena.mat");

    Eigen::ArrayXXd numberWhiteDots(1, 1);
    Eigen::ArrayXXd cumulWhiteDots(1, 1);
    file.read_mat("thumbNbWhiteDots", numberWhiteDots);
    file.read_mat("thumbValWhiteDots", cumulWhiteDots);

    Cell cell;

    cell.getGlobalCell(numberWhiteDots, cumulWhiteDots);

    std::cout << "global cell reading thumbnail \n" << cell.getGlobalCellOut() << std::endl;
}

void runAllTests() {
    
    START_UNIT_TEST;
    
    Eigen::MatioFile file("data/TestFilesMatMegarena.mat");

    Eigen::ArrayXXd numberWhiteDots(1, 1);
    Eigen::ArrayXXd cumulWhiteDots(1, 1);
    file.read_mat("thumbNbWhiteDots", numberWhiteDots);
    file.read_mat("thumbValWhiteDots", cumulWhiteDots);

    Cell cell;

    cell.getGlobalCell(numberWhiteDots, cumulWhiteDots);

    Eigen::Array33d globalCellTest;
    globalCellTest = cell.getGlobalCellOut();

    Eigen::Array33d globalCellRef;
    globalCellRef << 212.301, 190.254, 211.62,
            212.118, 189.728, 118.148,
            200.396, 180.896, 198.432;

    UNIT_TEST(areEqual(globalCellRef, globalCellTest, 0.001));
}

double speedGlobal(unsigned long testCount) {
    Eigen::MatioFile file("data/TestFilesMatMegarena.mat");

    Eigen::ArrayXXd numberWhiteDots(1, 1);
    Eigen::ArrayXXd cumulWhiteDots(1, 1);
    file.read_mat("thumbNbWhiteDots", numberWhiteDots);
    file.read_mat("thumbValWhiteDots", cumulWhiteDots);

    Cell cell;

    tic();

    for (unsigned long i = 0; i < testCount; i++) {
        cell.getGlobalCell(numberWhiteDots, cumulWhiteDots);
    }

    return toc(testCount);
}

int main(int argc, char** argv) {

    runAllTests();

    return EXIT_SUCCESS;
}