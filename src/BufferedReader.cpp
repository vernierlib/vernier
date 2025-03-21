/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "BufferedReader.hpp"

namespace vernier {

    BufferedReader::BufferedReader(const char * filename) {
        buffer = NULL;
        length = 0;
        load(filename);
    }

    BufferedReader::BufferedReader(const std::string & filename) {
        buffer = NULL;
        length = 0;
        load(filename.c_str());
    }

    BufferedReader::BufferedReader() {
        buffer = NULL;
        length = 0;
    }

    BufferedReader::~BufferedReader() {
        release();
    }

    void BufferedReader::load(const char * filename) {
        release();

        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            throw Exception("File not found or not accessible.");
        }

        // obtain file size:
        fseek(file, 0l, SEEK_END);
        length = ftell(file);
        rewind(file);

        // allocate memory to contain the whole file:
        buffer = (char*) malloc(sizeof (char)*(length + 1));
        if (buffer != NULL) {
            long result = fread(buffer, 1, length, file);
            buffer[result] = 0;
        } else {
            throw Exception("Memory allocation error in BufferedReader.");

        }
        fclose(file);
    }

    void BufferedReader::release() {
        if (buffer != NULL) {
            free(buffer);
            buffer = NULL;
            length = 0;
        }
    }

    char * BufferedReader::data() {
        return buffer;
    }

    long BufferedReader::size() {
        return length;
    }
    
    bool BufferedReader::equals(const BufferedReader & other) {
        return memcmp(buffer, other.buffer, length) == 0;
    }
    
    bool areFilesEqual(const std::string & filename1, const std::string & filename2) {
        BufferedReader b1(filename1);
        BufferedReader b2(filename2);
        return b1.equals(b2);
    }

}
