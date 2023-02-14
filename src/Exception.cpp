/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "Exception.hpp"

namespace Vernier {

    Exception::Exception(std::string errorMsg, std::string filename, int line) throw () {
        if (filename.empty()) {
            Exception::errorMsg = errorMsg;
        } else {
            Exception::errorMsg = errorMsg + " at line " + toString(line) + " in " + filename;
        }
    }

    const char* Exception::what() const throw () {
        return errorMsg.c_str();
    }

}