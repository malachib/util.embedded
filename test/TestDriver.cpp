#include "catch.hpp"

#include <Arduino.h>
#include "driver/gnss.h"

using namespace driver;

class DummyDriver : public IGNSS
{
public:
  VIRTUAL MetaData getMetaData() OVERRIDE
  {
    static MetaData m;
    m.deviceType = 10;
    return m;
  }
  VIRTUAL bool getGNSS(double* latitude, double* longitude,
    double* speed_kph,
    double* heading,
    double* altitude) OVERRIDE
  {
    return true;
  }
};

class TestClass
{
public:
  virtual void stuff()
  {
    INFO("Called stuff");
  }
};

class DummyDriver2 : public TestClass, IGNSS
{
public:
  VIRTUAL MetaData getMetaData() OVERRIDE
  {
    static MetaData m;
    m.deviceType = 11;
    return m;
  }
  VIRTUAL bool getGNSS(double* latitude, double* longitude,
    double* speed_kph,
    double* heading,
    double* altitude) OVERRIDE
  {
    return true;
  }
};

SCENARIO( "Driver implementation tests", "[driver-impl]" )
{
  GIVEN("A dummy GNSS driver")
  {
    DummyDriver d;

    REQUIRE(d.getMetaData().deviceType == 10);
  }
  GIVEN("A dummy multi-parent GNSS driver")
  {
    DummyDriver2 d;

    REQUIRE(d.getMetaData().deviceType == 11);
  }
}