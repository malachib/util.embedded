#include "catch.hpp"

#include <fact/lib.h>

using namespace util;

class DoubleWrapper
{
public:
  DoubleWrapper() {}

  DoubleWrapper(double val) : val(val) {}

  double val;

  virtual double operator()() const
  {
    return val;
  }
};

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
  GIVEN("A more complex but small buffer")
  {
    long _buffer[4];
    CircularBuffer<long> buffer(_buffer, 4);
    WHEN("Inserting and retrieving")
    {
      buffer.put(7);
      buffer.put(77);
      buffer.put(777);
      REQUIRE(buffer.get() == 7);
      REQUIRE(buffer.getPositionGet() == 1);
      REQUIRE(buffer.get() == 77);
      REQUIRE(buffer.getPositionGet() == 2);
      REQUIRE(buffer.get() == 777);
      REQUIRE(buffer.getPositionGet() == 3);
    }
  }
  GIVEN("A very small buffer, wrapping around")
  {
    uint8_t _buffer[3];
    CircularBuffer<uint8_t> buffer(_buffer, 3);

    buffer.put(7);
    REQUIRE(buffer.available() == 1);
    REQUIRE(buffer.getPosition() == 1);
    REQUIRE(buffer.getPositionGet() == 0);
    REQUIRE(buffer.get() == 7);
    buffer.put(8);
    REQUIRE(buffer.available() == 1);
    REQUIRE(buffer.getPosition() == 2);
    REQUIRE(buffer.getPositionGet() == 1);
    REQUIRE(buffer.get() == 8);
    buffer.put(9);
    REQUIRE(buffer.available() == 1);
    REQUIRE(buffer.getPosition() == 0);
    REQUIRE(buffer.getPositionGet() == 2);
    REQUIRE(buffer.get() == 9);
    buffer.put(10);
    REQUIRE(buffer.getPosition() == 1);
    REQUIRE(buffer.getPositionGet() == 0);
    buffer.put(11);
    REQUIRE(buffer.available() == 2);
    REQUIRE(buffer.getPosition() == 2);
    REQUIRE(buffer.getPositionGet() == 0);
    REQUIRE(buffer.get() == 10);
    REQUIRE(buffer.get() == 11);
    buffer.put(12);
    REQUIRE(buffer.available() == 1);
    REQUIRE(buffer.getPosition() == 0);
    REQUIRE(buffer.get() == 12);
    buffer.put(13);
    REQUIRE(buffer.available() == 1);
    REQUIRE(buffer.getPosition() == 1);
    REQUIRE(buffer.get() == 13);
  }
  GIVEN("A small and complex buffer, wrapping around")
  {
    DoubleWrapper _buffer[4];
    CircularBuffer<DoubleWrapper> buffer(_buffer, 3);
    DoubleWrapper dw;

    INFO("Item #1");
    buffer.put(1);
    REQUIRE(buffer.getPositionGet() == 0);
    REQUIRE(buffer.get()() == 1);
    INFO("Item #2");
    buffer.put(2);
    REQUIRE(buffer.getPositionGet() == 1);
    REQUIRE(buffer.get()() == 2);
    INFO("Item #3");
    buffer.put(3);
    REQUIRE(buffer.getPositionGet() == 2);
    REQUIRE(buffer.get()() == 3);
    INFO("Item #4");
    // this put line I think screws up the stack frame, position get shouldn't be 3 after this
    buffer.put(4);
    REQUIRE(buffer.getPositionGet() == 0);
    INFO("Item #4.1");
    REQUIRE(buffer.get().val == 4);
    INFO("Item #4.2");
    REQUIRE(buffer.getPositionGet() == 1);
    INFO("Item #5");
    buffer.put(5);
    INFO("Item #5.1");
    REQUIRE(buffer.get()() == 5);
  }
}
