#include "catch.hpp"

#include <fact/lib.h>

#include <fact/std/cstddef.hpp>

namespace fstd = util::std;

SCENARIO( "C++ std shim library tests", "[cpp_std_shim]" )
{
  GIVEN("layer 1 array")
  {
      int sizeof_size = sizeof(fstd::size_t);
      // This test requires a 64-bit ptr
      REQUIRE(sizeof_size == 8);

    //datetime.getDay();

  }
  GIVEN("layer 2 array")
  {
  }
  GIVEN("layer 3 array")
  {
  }
}
