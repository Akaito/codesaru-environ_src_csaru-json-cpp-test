/*
Copyright (c) 2015 Christopher Higgins Barrett

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifdef _MSC_VER
    #pragma warning(push)
    // fopen() unsafe.  Not using fopen_s for platform independence.
    #pragma warning(disable: 4996)
#endif

#include "TestCase2_1.hpp"

#include <JsonParser.hpp>
#include <DataMap.hpp>
#include <JsonParserCallbackForDataMap.hpp>
#include <JsonGenerator.hpp>

struct TestCaseData {
    const char *                        string;
    CSaruJson::JsonParser::ErrorStatus errorCode;
};

static const TestCaseData s_testDataArray[] = {
  //*
    {"string_test.json",                       CSaruJson::JsonParser::ErrorStatus::Done},
    {"out_test-string_test.json",              CSaruJson::JsonParser::ErrorStatus::Done}, // requires first test
    {"type_test0.json",                        CSaruJson::JsonParser::ErrorStatus::ParseError_ExpectedString},
    {"type_test0_1.json",                      CSaruJson::JsonParser::ErrorStatus::Done},
    {"type_test1.json",                        CSaruJson::JsonParser::ErrorStatus::ParseError_ExpectedValue},
    {"google_example-adjusted.json",           CSaruJson::JsonParser::ErrorStatus::ParseError_ExpectedValue},
    {"google_example-adjusted-2.json",         CSaruJson::JsonParser::ErrorStatus::Done},
    {"google_example-adjusted-2-smaller.json", CSaruJson::JsonParser::ErrorStatus::Done}
  //*/
};
static const int s_testCount = sizeof(s_testDataArray) / sizeof(s_testDataArray[0]);


bool TestCase2_1(void) {
    printf("\n\nTestCase2_1:\n");

    CSaruJson::JsonParser parser;

    for (int i = 0;  i < s_testCount;  ++i) {
        bool desiredResult = s_testDataArray[i].errorCode == CSaruJson::JsonParser::ErrorStatus::Done;
        printf("\n *********************************");
        printf("\n  Test 2_1.%d.  Should be %s.\n", i, desiredResult ? "valid" : "invalid");
        printf("  Parsing:\n%s\n\n", s_testDataArray[i].string);

        FILE * file;
        file = fopen(s_testDataArray[i].string, "rt");
        if (file == NULL) {
            printf("  Failed to open file.  Ignoring test\n");
            continue;
        }

        CSaruContainer::DataMap map;
        CSaruJson::JsonParserCallbackForDataMap callback(map.GetMutator());

        parser.Reset();
        parser.ParseEntireFile(file, NULL, 0, &callback);

        fclose(file);
        file = NULL;

        CSaruJson::JsonParser::ErrorStatus errorCode = parser.GetErrorCode();
        if (errorCode != s_testDataArray[i].errorCode) {
            printf(
                "  Failed test 2_1.%d.  JsonParser returned the wrong error code.\n  Was %d, expected %d.",
                errorCode, s_testDataArray[i].errorCode
            );
            abort();
            return false;
        }
        else {
            printf("  Passed test 2_1.%d.  JSON string was %s.\n", i, desiredResult ? "valid" : "invalid");
        }

        printf("  Map looked like:\n\n");
        CSaruContainer::DataMapReader reader = map.GetReader();
        CSaruJson::JsonGenerator::WriteToStream(&reader, stdout);

        //*
        // file output test
        reader = map.GetReader();
        std::string out_filename("out_test-");
        out_filename += s_testDataArray[i].string;
        CSaruJson::JsonGenerator::WriteToFile(&reader, out_filename.c_str());
        //*/
    }

    return true;
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
