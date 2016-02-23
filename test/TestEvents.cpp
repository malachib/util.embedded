#include "catch.hpp"

#include <iostream>
#include <cstdarg>

#include "EventManager.h"


int counter = 0;
char* eventValue;

void eventResponder(PropertyWithEvents<char*>* e)
{
  eventValue = *e;
  counter++;
}

SCENARIO( "Event/Handle manager tests", "[events]" )
{
  PubSTR_Property strProperty;
  std::string str = "TEST";

  GIVEN("A small event-property class")
  {
    strProperty.addUpdatedEvent(eventResponder);
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

  }
}
