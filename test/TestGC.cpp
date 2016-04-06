#include "catch.hpp"

#include <iostream>
#include <cstdarg>

#include <fact/gc.h>

using namespace FactUtilEmbedded;

uint8_t buffer[2048];
gc_manager gcm(buffer, 2048);

SCENARIO("GC & memory subsystem", "[gc]")
{
  GIVEN("A test")
  {

  }
}
