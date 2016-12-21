// temporarily disabled since we are moving away from Stream-shimming and
// into iostream-shimming
#ifdef DISABLED

#include "catch.hpp"

#include <Arduino.h>

SCENARIO( "IOStream tests", "[iostream]" )
{
  WHEN("Printing a normal string")
  {
    Serial.print(".");
  }
  WHEN("Printing a __FlashStringHelper")
  {
    Serial.print(F("."));
  }
  WHEN("Printing raw characters")
  {
    Serial.print('\n');
    Serial.print('\r');
  }
}
#endif
