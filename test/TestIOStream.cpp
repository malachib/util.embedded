#include "catch.hpp"

// Specifically testing iostream SHIM, not platform-native iostream
//#define __POSIX__
#define FEATURE_IOSTREAM_SHIM

#include <fact/iostream.h>

using namespace FactUtilEmbedded::std;

SCENARIO( "IOStream tests", "[iostream]" )
{
  WHEN("Printing a normal string")
  {
      cout << "iostream testing";
  }
  /*
  WHEN("Printing a __FlashStringHelper")
  {
      cout << F(".");
  }*/
  WHEN("Printing raw characters")
  {
      cout << '\n' << '\r';
  }
}
