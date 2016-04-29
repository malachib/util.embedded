#include "catch.hpp"

#include <fact/buffer.h>
#include <fact/lib.h>

using namespace util;

SCENARIO( "Container array tests", "[array]" )
{
  GIVEN("layer 1 array")
  {
    layer1::Array<int, 3> values;

    values[0] = 0;
    values[1] = 1;
    values[2] = 2;

    int counter = 0;

    for(auto v : values)
    {
      REQUIRE(v == counter++);
    }
  }
  GIVEN("layer 2 array")
  {
    int _values[3];

    layer2::Array<int, 3> values(_values);

    values[0] = 0;
    values[1] = 1;
    values[2] = 2;

    int counter = 0;

    for(auto v : values)
    {
      REQUIRE(v == counter++);
    }
  }
  GIVEN("layer 3 array")
  {
    int _values[3];

    layer3::Array<int> values(_values, 3);

    values[0] = 0;
    values[1] = 1;
    values[2] = 2;

    int counter = 0;

    for(auto v : values)
    {
      REQUIRE(v == counter++);
    }
  }
}
