/*
Copyright (c) 2016 Christopher Higgins Barrett

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

#include <cassert>

#include <csaru-datamap-cpp/DataMap.hpp>
#include <csaru-datamap-cpp/DataMapReader.hpp>
#include <csaru-json-cpp/JsonGenerator.hpp>

#include "TestCase0.hpp"

using CSaruDataMap::DataMap;
using CSaruDataMap::DataMapMutator;
using CSaruDataMap::DataMapReader;
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
