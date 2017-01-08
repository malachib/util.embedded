#include "catch.hpp"

#include <fact/lib.h>

#include <fact/std/cstddef.hpp>
#include <boost/asio/io_service.hpp>

namespace fstd = util::std;

SCENARIO( "C++ boost shim library tests", "[cpp_boost_shim]" )
{
  GIVEN("io_service test")
  {
      boost::asio::io_service ios;

    //datetime.getDay();

  }
  GIVEN("layer 2 array")
  {
  }
  GIVEN("layer 3 array")
  {
  }
}
