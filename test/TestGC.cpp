#include "catch.hpp"
#include "experimental/GC.h"
#include <string.h>

using namespace FactUtilEmbedded::experimental;

SCENARIO( "Garbage Collection/Virtual memory tests", "[gc]" )
{
    GIVEN("Synthetic test")
    {
        GC<1000> gc;

        auto gco = gc.alloc(100);

        REQUIRE(gc._allocated() == 100);
        REQUIRE(gc.available() == 900);

        uint8_t* buf = gc.lock(gco);

        strcpy((char*)buf, "Hi there");

        gc._free(gco);

        REQUIRE(gc._allocated() == 0);
        REQUIRE(gc.available() == 1000);
    }
}
