/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "Utils.hpp"

namespace vernier {

    /** \brief Exception class for error management.
     */
    class Exception : public std::exception {
    public:

        /** Construct exception with an error message
         */
        Exception(std::string errorMsg = "Unknown exception", std::string filename = "", int line = 0) throw ();

        virtual ~Exception() throw () {
        };

        /** Get string identifying exception
         *
         * \return error message
         */
        virtual const char* what() const throw ();

    protected:

        std::string errorMsg;

    };
}

#define ASSERT(condition) if (!(condition)) throw Exception(std::string("runtime error: ")+std::string(#condition),__FILE__,__LINE__);

#define ASSERT_MSG(condition, message) if (!(condition)) throw Exception(std::string("runtime error: ")+std::string(#message),__FILE__,__LINE__);

#define PRINT(variable) { std::cout << ">>> "<< #variable << " at line " << __LINE__ << " = " << variable << std::endl; }

#define IMSHOW(variable) { cv::imshow(#variable, variable); }

#define ARSHOW(variable) { arrayShow(#variable, variable); }

#endif