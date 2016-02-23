#include "catch.hpp"

#include <iostream>
#include <cstdarg>

#include "EventManager.h"


int counter = 0;
const char* eventValue;

void eventResponder(PropertyWithEvents<const char*>* e)
{
  eventValue = *e;
  counter++;
}

SCENARIO( "Event/Handle manager tests", "[events]" )
{
  GIVEN("A small event-property class")
  {
  PubSTR_Property strProperty;
  std::string str = "TEST";
  strProperty.addUpdatedEvent(eventResponder);

    WHEN("Doing basic operations")
    {
      strProperty.setValue("TEST");

      REQUIRE(counter == 1);
      REQUIRE(str == eventValue);

      strProperty.setValue("TEST2");
      str = "TEST2";

      REQUIRE(counter == 2);
      REQUIRE(str == eventValue);
    }
    WHEN("Using equality operator")
    {
      strProperty = "123";

      str = "123";

      REQUIRE(counter == 3);
      REQUIRE(str == eventValue);
    }
  }
}
