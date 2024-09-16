/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef BUFFEREDREADER_H
#define BUFFEREDREADER_H

#include "Common.hpp"

namespace Vernier {

    /** \brief Tool to read a document in a buffer and prepares it to be parsed. **/
    class BufferedReader {
    private:
        long length;
        char* buffer;

    public:

        /** BufferedReader constructor with a pointer to a filename
         *
         *   \param filename: pointer to the location of the filename
         **/
        BufferedReader(const char * filename);

        /** BufferedReader constructor with a referenced filename
         *
         *   \param filename: string filename to read
         **/
        BufferedReader(const std::string & filename);

        /** BufferedReader empty constructor
         **/
        BufferedReader();

        /** BufferedReader empty destructor
         **/
        ~BufferedReader();


        /** Loads the data contained in the file to a buffer
         *
         *   \param filename: filename of the document to read in a buffer
         **/
        void load(const char * filename);


        /** Frees the buffer
         **/
        void release();


        /** Returns the buffer of the read file
         **/
        char * data();

        /** Returns the size of the buffer read in from the file
         **/
        long size();

    };

}

#endif
