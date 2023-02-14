/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "TestBitmapPattern.hpp"

int main(int argc, char** argv) {

    try {
        Vernier::TestBitmapPattern::runAllTests();
    } catch (const Vernier::Exception& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}