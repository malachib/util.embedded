#include "catch.hpp"

#include <iostream>
#include <cstdarg>

#include "EventManager.h"


int counter = 0;
int counter2 = 0;
const char* eventValue;
const char* eventValue2;

void eventResponder(PropertyWithEvents<const char*>* e)
{
  eventValue = *e;
  counter++;
  //printf("eventResponder1: %d\r\n", counter);
}

void eventResponder2(PropertyWithEvents<const char*>* e)
{
  counter2++;
  eventValue2 = *e;
  //printf("eventResponder2: %d\r\n", counter2);
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
      strProperty.updated += eventResponder2;

      REQUIRE(counter == 2);

      strProperty = "123";

      str = "123";

      REQUIRE(counter == 3);
      REQUIRE(str == eventValue);
      REQUIRE(counter2 == 1);
      REQUIRE(str == eventValue2);
    }
  }
}
