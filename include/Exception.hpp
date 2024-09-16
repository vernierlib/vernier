/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "Utils.hpp"

namespace Vernier {

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

#endif