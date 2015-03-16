/*
Copyright (c) 2011 Christopher Higgins Barrett

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "TestCase2_0.hpp"

#include <stdio.h>

#include <JsonParser.hpp>
#include <DataMap.hpp>
#include <JsonParserCallbackForDataMap.hpp>
#include <JsonGenerator.hpp>

struct TestCaseData {
  const char* string;
  bool is_valid;
};

static const TestCaseData s_testDataArray[] = {
    //*
    {"{\"\": \"\" }", true},
    {" { \"foo\" :\"bar\"}", true},
    {"{\"foo2\": \"bar2\" \"bar2 again\" }", false},
    {"{\"foo2\" \"another name\" : \"bar2\" }", false},
    {"{\"missing value\":}", false},
    {"{\"missing value\": }", false},
    {"{\"missing value\": ,}", false},
    {"{\"missing value\": \n}", false},
    // null literal tests
    {"{\"value on next line\": \nnull}", true},
    {"{\"value on next line\": \n null\n}", true},
    {"{\"null\":null}", true},
    {"{\"bad null\":nuLl}", false},
    {"{\"bad null\":nulll}", false},
    {"{\"bad null\":nu}", false},
    {"{\"bad null\":n}", false},
    {"{\"bad null\":n }", false},
    {"{\"bad null\":nul l }", false},
    {"{ \" yan \" : null }", true},
    // nested object tests
    {"{ \" obj \" : {} }", true},
    {"{ \" obj \" : {\"obj2\":{}} }", true},
    // true literal tests
    {"{\"value on next line\": \ntrue}", true},
    {"{\"value on next line\": \n true\n}", true},
    {"{\"True\":true}", true},
    {"{\"bad true\":trUe}", false},
    {"{\"bad true\":truee}", false},
    {"{\"bad true\":tr}", false},
    {"{\"bad true\":t}", false},
    {"{\"bad true\":t }", false},
    {"{\"bad true\":tru e }", false},
    {"{\"bad true\":True }", false},
    {"{\"bad true\":TRUE }", false},
    {"{ \" yat \" : true }", true},
    // false literal tests
    {"{\"value on next line\": \nfalse}", true},
    {"{\"value on next line\": \n false\n}", true},
    {"{\"True\":false}", true},
    {"{\"bad true\":faLse}", false},
    {"{\"bad true\":falsee}", false},
    {"{\"bad true\":fa}", false},
    {"{\"bad true\":f}", false},
    {"{\"bad true\":f }", false},
    {"{\"bad true\":fal se }", false},
    {"{\"bad true\":False }", false},
    {"{\"bad true\":FALSE }", false},
    {"{ \" yaf \" : false }", true},
    // integer tests
    {"{\"answer\": 42}", true},
    {"{\"answer\": 42 }", true},
    {"{\"answer\": 042}", false},
    {"{\"answer\": 0}", true},
    {"{\"answer\": 0 }", true},
    {"{\"answer\": 00 }", false},
    {"{\"answer\": 2048}", true},
    {"{\"answer\": 3}", true},
    {"{\"answer\": 3 }", true},
    {"{\"answer\": 2049 }", true},
    {"{\"answer\": 99999999999999999 }", true},
    {"{\"answer\": 99999999999999999}", true},
    {"{\"answer\": 3 3}", false},
    {"{\"answer\": 0 3}", false},
    {"{\"answer\": 3 0}", false},
    {"{\"answer\": 3 3 }", false},
    {"{\"answer\": 0 3 }", false},
    {"{\"answer\": 3 0 }", false},
    {"{\"answer\": 42e}", false},
    {"{\"answer\": 42E}", false},
    {"{\"answer\": 42Y}", false},
    {"{\"answer\": 4a2Y}", false},
    {"{\"answer\": x02}", false},
    // negative integer tests
    {"{\"answer\": -42}", true},
    {"{\"answer\": -42 }", true},
    {"{\"answer\": -042}", false},
    {"{\"answer\": -0}", true},
    {"{\"answer\": -0 }", true},
    {"{\"answer\": -00 }", false},
    {"{\"answer\": -2048}", true},
    {"{\"answer\": -3}", true},
    {"{\"answer\": -3 }", true},
    {"{\"answer\": -2049 }", true},
    {"{\"answer\": -99999999999999999 }", true},
    {"{\"answer\": -99999999999999999}", true},
    {"{\"answer\": -3 3}", false},
    {"{\"answer\": -0 3}", false},
    {"{\"answer\": -3 0}", false},
    {"{\"answer\": -3 3 }", false},
    {"{\"answer\": -0 3 }", false},
    {"{\"answer\": -3 0 }", false},
    {"{\"answer\": 3 -3}", false},
    {"{\"answer\": 0 -3}", false},
    {"{\"answer\": 3 -0}", false},
    {"{\"answer\": 3 -3 }", false},
    {"{\"answer\": 0 -3 }", false},
    {"{\"answer\": 3 -0 }", false},
    {"{\"answer\": -3 -3}", false},
    {"{\"answer\": -0 -3}", false},
    {"{\"answer\": -3 -0}", false},
    {"{\"answer\": -3 -3 }", false},
    {"{\"answer\": -0 -3 }", false},
    {"{\"answer\": -3 -0 }", false},
    // float tests
    {"{\"answer\": 0.0}", true},
    {"{\"answer\": 42.0}", true},
    {"{\"answer\": 42.1 }", true},
    {"{\"answer\": 0. }", false},
    {"{\"answer\": 4. }", false},
    {"{\"answer\": 456. }", false},
    {"{\"answer\": 042.5}", false},
    {"{\"answer\": 0.123 }", true},
    {"{\"answer\": 00.7 }", false},
    {"{\"answer\": 2048.9}", true},
    {"{\"answer\": 3.3}", true},
    {"{\"answer\": 3.666 }", true},
    {"{\"answer\": 2049.42 }", true},
    {"{\"answer\": 99999999999999999.9999999999999999999999 }", true},
    {"{\"answer\": 99999999999999999.9999999999999999999999}", true},
    {"{\"answer\": 3.0 3.0}", false},
    {"{\"answer\": 0.0 3.0}", false},
    {"{\"answer\": 3.0 0.0}", false},
    {"{\"answer\": 3.0 3.0 }", false},
    {"{\"answer\": 0.0 3.0 }", false},
    {"{\"answer\": 3.0 0.0 }", false},
    {"{\"answer\": 42.0e}", false},
    {"{\"answer\": 42.0E}", false},
    {"{\"answer\": 42.0Y}", false},
    {"{\"answer\": 4a2Y}", false},
    {"{\"answer\": x02}", false},
    // negative float tests
    {"{\"answer\": -42.0}", true},
    {"{\"answer\": -42.7 }", true},
    {"{\"answer\": -042.5}", false},
    {"{\"answer\": -0.2}", true},
    {"{\"answer\": -0.1 }", true},
    {"{\"answer\": -00.249 }", false},
    {"{\"answer\": -2048.9}", true},
    {"{\"answer\": -3.5}", true},
    {"{\"answer\": -3.23939 }", true},
    {"{\"answer\": -2049.2049 }", true},
    {"{\"answer\": -99999999999999999.99999999999999999 }", true},
    {"{\"answer\": -99999999999999999.99999999999999999}", true},
    {"{\"answer\": -3.0 3.0}", false},
    {"{\"answer\": -0.0 3.0}", false},
    {"{\"answer\": -3.0 0.0}", false},
    {"{\"answer\": -3.0 3.0 }", false},
    {"{\"answer\": -0.0 3.0 }", false},
    {"{\"answer\": -3.0 0.0 }", false},
    {"{\"answer\": 3.0 -3.0}", false},
    {"{\"answer\": 0.0 -3.0}", false},
    {"{\"answer\": 3.0 -0.0}", false},
    {"{\"answer\": 3.0 -3.0 }", false},
    {"{\"answer\": 0.0 -3.0 }", false},
    {"{\"answer\": 3.0 -0.0 }", false},
    {"{\"answer\": -3.0 -3.0}", false},
    {"{\"answer\": -0.0 -3.0}", false},
    {"{\"answer\": -3.0 -0.0}", false},
    {"{\"answer\": -3.0 -3.0 }", false},
    {"{\"answer\": -0.0 -3.0 }", false},
    {"{\"answer\": -3.0 -0.0 }", false},
    // array tests
    {"{\"array\": [] }", true},
    {"{\"array\": [ ] }", true},
    {"{\"array\":[]}", true},
    {"{\"array\":[] }", true},
    {"{\"array\": []}", true},
    {"{\"array\": [4] }", true},
    {"{\"array\": [ 4 ] }", true},
    {"{\"array\": [ 4] }", true},
    {"{\"array\": [4 ] }", true},
    {"{\"array\": [4,2] }", true},
    {"{\"array\": [4, 2] }", true},
    {"{\"array\": [4 ,2] }", true},
    {"{\"array\": [4 , 2] }", true},
    {"{\"array\": [4 , 2} }", false},
    {"{\"array\": [4 , \"string\", 3.14, true, false, null, 0] }", true},
    {"{\"array\": [4 , {\"float\": 3.14}, [], [false, null, 0]] }", true},
    {"{\"array\": [ {} ] }", true},
    //*/
    {"{\"array\": [ {}, 8 ] }", true},
    {"{\"array\": [ {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {} ] }", true},
    {"{\"array\": [ [], [], [], [], [], [], [], [], [], [], [] ] }", true},
    {"{\"array\": [ {}, {\"obj\":{}}, {}, {}, {}, {}, {}, {}, {}, {}, {} ] }", true},
    {"{\"array\": [ [], [], [], [], [], [] ] }", true},
    {"{\"array\": [ [[[[[[]]]]]], [[[[]]], [[],[[]]], []], [], [], [], [] ] }", true}
    //*/
};
static const int s_testCount = sizeof(s_testDataArray) / sizeof(s_testDataArray[0]);


bool TestCase2_0(void) {
    printf("\n\nTestCase2_0:\n");

    CSaruJson::JsonParser parser;

    for (int i = 0;  i < s_testCount;  ++i) {
        bool desired_result = s_testDataArray[i].is_valid;
        printf("\n*********************************");
        printf("\n  Test 2_0.%d.  Should be %s.\n", i, desired_result ? "valid" : "invalid");
        printf("  Looks like:\n%s\n\n", s_testDataArray[i].string);

        CSaruContainer::DataMap map;
        CSaruJson::JsonParserCallbackForDataMap callback(map.GetMutator());

        parser.Reset();
        parser.ParseBuffer(s_testDataArray[i].string, strlen(s_testDataArray[i].string), &callback);
        bool passed = parser.GetErrorCode() < CSaruJson::JsonParser::ErrorStatus::Error_Unspecified;
        if (passed != desired_result) {
            //printf("Failed test #%d.  Should have %s.  Test data was:\n%s\n",
            printf(
                "  Failed test 2_0.%d.  JSON string should have been %s.\n",
                i,
                desired_result ? "valid" : "invalid"
            );
            abort();
            return false;
        } else {
            printf("  Passed test 2_0.%d.  JSON string was %s.\n", i, desired_result ? "valid" : "invalid");
        }

        printf("  Map looked like:\n\n");
        CSaruContainer::DataMapReader reader = map.GetReader();
        CSaruJson::JsonGenerator::WriteToStream(&reader, stdout);
    }

    return true;
}
