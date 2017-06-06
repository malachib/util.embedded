#include "catch.hpp"
#include "experimental/GC.h"

using namespace FactUtilEmbedded::experimental;

SCENARIO( "Garbage Collection/Virtual memory tests", "[gc]" )
{
    GIVEN("Synthetic test")
    {
        GC<1000> gc;

        auto gco = gc.alloc(100);

        gc._free(gco);
    }
}
