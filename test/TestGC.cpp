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

        auto gco2 = gc.alloc(50);

        uint8_t* buf = gc.lock(gco);

        strcpy((char*)buf, "Hi there");

        uint8_t* buf2 = gc.lock(gco);

        REQUIRE(gc._allocated() == 150);
        REQUIRE(gc.available() == 850);

        gc._free(gco);
        gc._free(gco2);

        REQUIRE(gc._allocated() == 0);
        REQUIRE(gc.available() == 1000);
    }
}
