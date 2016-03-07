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


class EventFiringClass
{
  LOCAL_EVENT(EventFiringClass);
public:

  Event testEvent1;

  void fireTestEvent1() { testEvent1(this); }
};

int eventResponder3_counter = 0;

void eventResponder3(EventFiringClass* source)
{
  eventResponder3_counter++;
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
    WHEN("Using EventFiringClass")
    {
      EventFiringClass efc;

      efc.testEvent1 += eventResponder3;
      efc.fireTestEvent1();

      REQUIRE(eventResponder3_counter == 1);

      efc.fireTestEvent1();

      REQUIRE(eventResponder3_counter == 2);
    }
  }
}
