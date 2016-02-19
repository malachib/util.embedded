#include "catch.hpp"

#include <iostream>
#include <cstdarg>

#include <driver/core.h>

class VariableInputClass
{
public:
  VariableInputClass(void* parent, ...)
  {
    va_list args;
    va_start(args, parent);

    testValue = va_arg(args, int);

    va_end(args);
  }

  int testValue;
};

SCENARIO( "Driver core tests", "[driver-core]" )
{
  GIVEN("A variadic constructor")
  {
    VariableInputClass testClass(NULL, 777);

    REQUIRE(testClass.testValue == 777);
  }
}
