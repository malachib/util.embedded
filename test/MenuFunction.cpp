#include "catch.hpp"

#include <Arduino.h>
#include <fact/MenuFunction.h>

using namespace util;

int testFunc1(int value)
{
  INFO("Got here with value: ")
  INFO(value);
  return -1;
}

//Console console;

SCENARIO( "MenuFunction tests", "[menu-function]" )
{
  MenuFunction<int, int> menuFunction1(testFunc1);

  GIVEN("A string to integer conversion")
  {
    StringConverter<int> converter;
    auto value = converter.fromString("123");

    REQUIRE(value == 123);
  }
  GIVEN("The simplest case MenuFunction int, int")
  {

  }
}
