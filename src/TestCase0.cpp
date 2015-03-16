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

#include "TestCase0.hpp"
#include <assert.h>
#include <DataMap.hpp>
#include <DataMapReader.hpp>
#include <JsonGenerator.hpp>

using CSaruContainer::DataMap;
using CSaruContainer::DataMapMutator;
using CSaruContainer::DataMapReader;
using CSaruJson::JsonGenerator;

void TestCase0(void) {
  printf("TestCase 0:\n");
  
  /*
  foo.PushGroup("Barrel");
    foo.Write("Monkey0", 42);
    foo.Read("Monkey1", &i);
  foo.PopGroup();
  //*/
  
  int monkey0_value = -1024;
  const int monkey0_value_desired = 42;
  bool monkey1_value = false;
  const bool monkey1_value_desired = true;
  float monkey2_value = 0.1f;
  const float monkey2_value_desired = 3.14159265358979f;
  
  const char* json_out_file = "TestCase0.json";
  
  DataMap dmap;
  DataMapMutator mut = dmap.GetMutator();
  mut.ToChild("Barrel");
    mut.ToChild("Monkey0");
      mut.Write(monkey0_value_desired);
      mut.ReadIntSafe(&monkey0_value);
      assert(monkey0_value == monkey0_value_desired &&
             "Write/ReadIntSafe failed.");
      monkey0_value = mut.ReadInt();
      assert(monkey0_value == monkey0_value_desired &&
             "Write/ReadInt failed.");
      
      mut.ToNextSibling();
      mut.Write("Monkey1", monkey1_value_desired);
      monkey1_value = mut.ReadBoolWalk();
      assert(monkey1_value == monkey1_value_desired &&
             "ToNextSibling, Write/ReadBoolWalk failed.");
      
      mut.Write("Monkey2", monkey2_value_desired);
      if (!mut.ReadFloatSafe(&monkey2_value)) {
        assert(false && "Write/ReadFloatSafe failed.");
      }
      assert(monkey2_value == monkey2_value_desired &&
             "Write/ReadFloatSafe failed.");
    mut.PopNode();
  mut.PopNode();
  
  printf("  \"Barrel\" { \"Monkey0\" : %d ...\n", monkey0_value_desired);
  if (monkey0_value == monkey0_value_desired)
    printf("    Pass.\n");
  else
    printf("    FAIL. Read %d\n", monkey0_value);
  
  printf("Writing to file [%s].\n", json_out_file);
  DataMapReader reader = dmap.GetReader();
  bool write_to_file_result =
      JsonGenerator::WriteToFile(&reader, json_out_file);
  printf("  %s.\n", write_to_file_result ? "Success" : "Failure");
}
