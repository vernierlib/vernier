/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "TestPattern.hpp"

using namespace Vernier;
using namespace std;

int main(int argc, char** argv) {

    try {
        Vernier::TestPattern::runAllTests();
    } catch (const Exception& e) {
        cout << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}