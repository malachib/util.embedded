#include "catch.hpp"

#include <properties.h>
#include <fact/lib.h>

using namespace util;

template <template <class T> class TProperty, class T = int>
void test()
{
  T value = 5;

  TProperty<T> prop(&value);

  REQUIRE(prop == 5);

  prop.set(7);

  REQUIRE(value == 7);
  REQUIRE(prop == 7);

  prop = 4;

  REQUIRE(value == 4);
}

SCENARIO("Properties test", "[properties]")
{
  GIVEN("layer 2")
  {
    test<layer2::Property>();
  }
  GIVEN("layer 3")
  {
    test<layer3::Property>();
  }
  GIVEN("layer 5")
  {
    test<layer5::Property>();
  }
}
