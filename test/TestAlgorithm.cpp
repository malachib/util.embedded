#include "catch.hpp"
#include "fact/algorithm.h"

namespace fstd = FactUtilEmbedded::std;

SCENARIO( "Testing <algorithm> shim code", "[std-algorithm]" )
{
    char buf1[] = "Hi there";
    char buf2[100];

    GIVEN("std::copy (to the left) behaviors")
    {
        fstd::copy(buf1, buf1 + 10, buf2);

        REQUIRE(strcmp(buf1, buf2) == 0);
    }
    GIVEN("std::copy (to the right) behaviors")
    {
        fstd::copy_backward(buf1, buf1 + 10, buf2 + 10);

        REQUIRE(strcmp(buf1, buf2) == 0);
    }
}
