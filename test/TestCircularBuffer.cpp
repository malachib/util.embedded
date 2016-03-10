#include "catch.hpp"

#include <fact/lib.h>

using namespace util;

SCENARIO( "Circular Buffer tests", "[circular-buffer]" )
{
  GIVEN("A buffer of 1024 elements")
  {
    uint16_t _buffer[1024];
    CircularBuffer<uint16_t> buffer(_buffer, 1024);

    WHEN("Inserting")
    {
      INFO("Inserting 123");
      buffer.put(123);
      INFO("Inserting 456");
      buffer.put(456);
      REQUIRE(buffer.available() == 2);
    }

    WHEN("Pulling back out")
    {
      INFO("Inserting 123");
      buffer.put(123);
      INFO("Inserting 456");
      buffer.put(456);
      REQUIRE(buffer.available() == 2);
      auto value = buffer.get();
      REQUIRE(value == 123);
      value = buffer.get();
      REQUIRE(value == 456);
    }
  }
  GIVEN("A buffer of 4 elements")
  {
    char _buffer[4];
    CircularBuffer<char> buffer(_buffer, 4);

    WHEN("Inserting")
    {
      INFO("Inserting A");
      buffer.put('A');
      REQUIRE(buffer.available() == 1);
      INFO("Inserting B");
      buffer.put('B');
      REQUIRE(buffer.available() == 2);
      INFO("Inserting C");
      buffer.put('C');
      REQUIRE(buffer.available() == 3);
      INFO("Inserting D");
      buffer.put('D');
      REQUIRE(buffer.available() == 4);
      INFO("Inserting E");
      buffer.put('E');
      REQUIRE(buffer.available() == 4);
      INFO("Inserting F");
      buffer.put('F');
      REQUIRE(buffer.available() == 4);
    }
  }
}
