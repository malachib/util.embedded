#include "catch.hpp"

#include <fact/buffer.h>

using namespace FactUtilEmbedded;

SCENARIO( "Memry Container tests", "[buffer]" )
{
  GIVEN("layer 1 testing")
  {
    layer1::MemoryContainer<1024> buf;
    
    buf.clear();
    
    REQUIRE(buf.getSize() == 1024);
  }
  GIVEN("layer 2 testing")
  {
    uint8_t _buffer[1024];
    layer2::MemoryContainer<1024> buf(_buffer);
    
    buf.clear();
    buf.getData();
    
    REQUIRE(buf.getSize() == 1024);
  }
  GIVEN("layer 3 testing")
  {
    uint8_t _buffer[1024];
    layer3::MemoryContainer<> buf(_buffer, 1024);
    
    buf.clear();
    
    REQUIRE(buf.getSize() == 1024);
  }
  GIVEN("layer 5 testing")
  {
    uint8_t _buffer[1024];
    layer5::MemoryContainer<> buf(_buffer, 1024);
    
    REQUIRE(buf.getSize() == 1024);
  }
  {
/*    CircularBuffer<uint16_t> buffer(_buffer, 1024);

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
    } */
  }
}
