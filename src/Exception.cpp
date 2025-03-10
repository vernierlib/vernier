/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "Exception.hpp"

namespace vernier {

    Exception::Exception(std::string errorMsg, std::string filename, int line) throw () {
        if (filename.empty()) {
            Exception::errorMsg = "[Vernier exception] " + errorMsg;
        } else {
            Exception::errorMsg = "[Vernier exception in " + filename + " at line " + to_string(line) + "] " + errorMsg;
        }
    }

    const char* Exception::what() const throw () {
        return errorMsg.c_str();
    }

}