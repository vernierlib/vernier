/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018 CNRS, ENSMM, UFC.
 */

#ifndef TESTBUFFEREDREADER_HPP
#define TESTBUFFEREDREADER_HPP

#include "BufferedReader.hpp"
#include "UnitTest.hpp"
#include "rapidjson/document.hpp"

#include <fstream>

namespace Vernier {

    class TestBufferedReader {
    public:

        static void main() {

            BufferedReader bufferedReader("test.json");
            std::cout << bufferedReader.data() << std::endl << std::endl;

            rapidjson::Document document;
            document.Parse(bufferedReader.data());
            assert(document.IsObject());


            assert(document.HasMember("hello"));
            assert(document["hello"].IsString());
            printf("hello = %s\n", document["hello"].GetString());

            assert(document["t"].IsBool());
            printf("t = %s\n", document["t"].GetBool() ? "true" : "false");

            printf("n = %s\n", document["n"].IsNull() ? "null" : "?");

            assert(document["i"].IsNumber());
            assert(document["i"].IsInt());
            printf("i = %d\n", document["i"].GetInt());

            assert(document["pi"].IsNumber());
            assert(document["pi"].IsDouble());
            printf("pi = %g\n", document["pi"].GetDouble());

            const rapidjson::Value& a = document["a"];
            assert(a.IsArray());
            for (rapidjson::SizeType i = 0; i < a.Size(); i++) { // Uses SizeType instead of size_t
                printf("a[%d] = %d\n", i, a[i].GetInt());
            }

            assert(document["mat"].IsArray());
            for (rapidjson::SizeType i = 0; i < document["mat"].Size(); i++) {// Uses SizeType instead of size_t
                const rapidjson::Value& ligne = document["mat"][i];
                assert(ligne.IsArray());
                for (rapidjson::SizeType j = 0; j < ligne.Size(); j++) {
                    printf("a[%d][%d] = %d\n", i, j, ligne[j].GetInt());
                }
            }
        }

        static void main2() {

            std::string filename("testBitmapPattern.json");
            std::string description;
            std::string date;
            std::string author;
            double period;
            std::string unit;
            int nRows, nCols;
            Eigen::ArrayXXi bitmap;

            BufferedReader bufferedReader(filename);

            rapidjson::Document document;
            document.Parse(bufferedReader.data());
            if (!document.IsObject()) {
                throw Exception(filename + " is not a valid JSON file.");
            }
            if (document.MemberBegin() == document.MemberEnd()) {
                throw Exception(filename + " is empty.");
            }
            std::cout << document.MemberBegin()->name.GetString() << std::endl;
            if (document.MemberBegin()->name.GetString() != std::string("BitmapPatternLayout")) {// && document["class"].GetString().compare == std::string("bitmapPattern").) {
                throw Exception(filename + " is not a valid bitmap pattern file.");
            }
            
            rapidjson::Value& value = document.MemberBegin()->value;
            if (value.HasMember("description") && value["description"].IsString()) {
                description = value["description"].GetString();
            } else {
                description = "";
            }


            if (document.HasMember("date") && document["date"].IsString()) {
                date = document["date"].GetString();
            } else {
                date = "";
            }

            if (document.HasMember("author") && document["author"].IsString()) {
                author = document["author"].GetString();
            } else {
                author = "";
            }

            if (document.HasMember("period") && document["period"].IsDouble()) {
                period = document["period"].GetDouble();
            } else {
                throw Exception(filename + " is not a valid bitmap pattern file, the period is missing or has a wrong format.");
            }

            if (document.HasMember("unit") && document["unit"].IsString()) {
                unit = document["unit"].GetString();
            } else {
                unit = "";
            }

            if (document.HasMember("bitmap") && document["bitmap"].IsArray()) {
                nRows = document["bitmap"].Size();
            } else {
                throw Exception(filename + " is not a valid bitmap pattern file, the bitmap is missing or has a wrong format.");
            }

            if (document["bitmap"][0].IsArray() && document["bitmap"][0].Size() > 0) {
                nCols = document["bitmap"][0].Size();
            } else {
                throw Exception(filename + " is not a valid bitmap pattern file, the first row of the bitmap has a wrong format.");
            }

            bitmap.resize(nRows, nCols);
            for (rapidjson::SizeType row = 0; row < nRows; row++) {
                const rapidjson::Value& value = document["bitmap"][row];
                if (value.IsArray() && value.Size() == nCols) {
                    for (rapidjson::SizeType col = 0; col < nCols; col++) {
                        if (value[col].IsInt()) {
                            bitmap(row, col) = value[col].GetInt();
                        } else {
                            Exception(filename + " is not a valid bitmap pattern file, the row " + toString(row) + " of the bitmap has a wrong format");
                        }
                    }
                } else {
                    throw Exception(filename + " is not a valid bitmap pattern file, the row " + toString(row) + " of the bitmap has a wrong format");
                }
            }

            std::cout << filename << std::endl;
            std::cout << description << std::endl;
            std::cout << date << std::endl;
            std::cout << author << std::endl;
            std::cout << period << std::endl;
            std::cout << unit << std::endl;
            std::cout << nRows << std::endl;
            std::cout << nCols << std::endl;
            std::cout << bitmap << std::endl;

        }

        static void runAllTests() {
            std::ofstream mFile("runAllTestFile.json");
            if (mFile.is_open()) {
                mFile << "{\n\t\"hello\": \"world\",\n\t\"t\": true,\n\t\"f\": false,\n\t\"n\": null,\n\t\"i\": 123,\n\t\"pi\": 3.1416,\n\t\"a\": [1, 2, 3, 4]\n}";
                mFile.close();
            } else {
                throw Exception("Can't write file");
            }

            BufferedReader bufferedReader("runAllTestFile.json");

            rapidjson::Document document;
            document.Parse(bufferedReader.data());
            UNIT_TEST(document.IsObject());

            UNIT_TEST(document.HasMember("hello"));
            UNIT_TEST(document["hello"].IsString());
            UNIT_TEST(document["hello"].GetString() == std::string("world"));

            UNIT_TEST(document["t"].IsBool());
            UNIT_TEST(document["t"].GetBool() == true);

            UNIT_TEST(document["f"].IsBool());
            UNIT_TEST(document["f"].GetBool() == false);

            UNIT_TEST(document["n"].IsNull());

            UNIT_TEST(document["i"].IsNumber());
            UNIT_TEST(document["i"].IsInt());
            UNIT_TEST(document["i"].GetInt() == 123);

            UNIT_TEST(document["pi"].IsNumber());
            UNIT_TEST(document["pi"].IsDouble());
            UNIT_TEST(document["pi"].GetDouble() == 3.1416);

            int testedArray[4] = {1, 2, 3, 4};

            const rapidjson::Value& a = document["a"];
            UNIT_TEST(a.IsArray());
            for (rapidjson::SizeType i = 0; i < a.Size(); i++) { // Uses SizeType instead of size_t
                UNIT_TEST(a[i].GetInt() == testedArray[i]);
            }

            remove("runAllTestFile.json");
        }

        static double speed(unsigned long testCount) {
            return -1;
        }

    };

}

#endif
