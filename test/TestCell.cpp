/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "TestCell.hpp"

int main(int argc, char** argv) {

    try {
        Vernier::TestCell::runAllTests();
    } catch (const Vernier::Exception& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}