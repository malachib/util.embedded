#include "catch.hpp"

#include <Arduino.h>
#include <fact/MenuFunction.h>

using namespace util;

int testFunc1(int value)
{
  REQUIRE(value == 123);
  //INFO("Got here with value: ")
  //INFO(value);
  return -1;
}

class DummyMenuFunction : public MenuFunction<int, int>
{
public:
  DummyMenuFunction(_handler handler) : MenuFunction<int, int>(handler) {}

  void test1()
  {
    const char* _p[] = { "123" };
    Parameters p((char**)_p, 1, NULL);

    handleCommand(p);
  }
};

//Console console;

SCENARIO( "MenuFunction tests", "[menu-function]" )
{
  //MenuFunction<int, int> menuFunction1(testFunc1);
  DummyMenuFunction menuFunction1(testFunc1);

  GIVEN("A string to integer conversion")
  {
    StringConverter<int> converter;
    auto value = fromString<int>("123");

    REQUIRE(value == 123);
  }
  GIVEN("The simplest case MenuFunction int, int")
  {
    menuFunction1.test1();
  }
}
