#include "catch.hpp"
#include "experimental/GC.h"
#include <string.h>

using namespace FactUtilEmbedded::experimental;

GC<1000> __gc;

namespace FactUtilEmbedded { namespace experimental {
GC_base& _gc = ::__gc;

}}

SCENARIO( "Garbage Collection/Virtual memory tests", "[gc]" )
{
    GC<1000> gc;


    GIVEN("Synthetic test")
    {
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
    GIVEN("Synthetic test 2")
    {
        auto gco = gc.alloc(100);
        auto gco2 = gc.alloc(50);

        gc._free(gco);
        gc._free(gco2);

        // this crashes
        gc.recombine_all();
    }
    GIVEN("GCPointer test")
    {
        auto gcp = gc.alloc2<char>(100);

        char* str = gcp.lock();

        strcpy(str, "Hi2u");

        gcp.unlock();

        REQUIRE(gc.available() == 900);
    }
}
