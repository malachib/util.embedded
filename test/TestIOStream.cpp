#include "catch.hpp"

// Specifically testing iostream SHIM, not platform-native iostream
//#define __POSIX__
#define FEATURE_IOSTREAM_SHIM

#include <fact/iostream.h>

using namespace FactUtilEmbedded::std;

struct Special
{
    const char* value;
};

template <class TOStream>
inline TOStream& operator <<(TOStream& out, Special& arg)
{
    auto& retVal = out.write(arg.value, strlen(arg.value));
    // PRESUMABLY: C++ std iostreams use basic_ostream specializations to get around
    // the issue experienced here with return retVal (I'm using actual
    // inheritance from basic_ostream, not specialization)
    //return retVal;
    return out;
}


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
  WHEN("Testing special overload")
  {
      Special special;

      special.value = "Special value output\r\n";

      cout  << special;
  }
}
