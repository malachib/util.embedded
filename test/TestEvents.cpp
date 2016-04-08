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

std::string _fluidParameter = "fluid parameter";

class EventFiringClass
{
  LOCAL_EVENT(EventFiringClass);
public:

  Event testEvent1;

  void fireTestEvent1() { testEvent1(this, _fluidParameter.c_str()); }
};

int eventResponder3_counter = 0;
int eventResponder4_counter = 0;

void eventResponder4(EventFiringClass* source, va_list argp)
{
  auto fluidParameter = va_arg(argp, const char*);
  //INFO("Fluid parameter = " << fluidParameter);
  //printf("\r\n Firing: %s", fluidParameter);
  REQUIRE(fluidParameter != NULL);
  REQUIRE(_fluidParameter == fluidParameter);
  eventResponder4_counter++;
}

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
    WHEN("Using EventFiringClass Linear")
    {
      EventFiringClass efc;

      efc.testEvent1 += eventResponder3;
      efc.fireTestEvent1();

      REQUIRE(eventResponder3_counter == 1);

      efc.fireTestEvent1();

      REQUIRE(eventResponder3_counter == 2);
    }
    WHEN("Using EventFiringClass Add/Clear/Add")
    {
      eventResponder3_counter = 0;
      EventFiringClass efc;

      efc.testEvent1 += eventResponder3;
      efc.testEvent1.clear();
      efc.testEvent1 += eventResponder3;
      WHEN("Invoking test event 1")
      {
        efc.fireTestEvent1();
      }

      REQUIRE(eventResponder3_counter == 1);

      efc.fireTestEvent1();

      REQUIRE(eventResponder3_counter == 2);
    }
    WHEN("Removing one from a 1-long event list")
    {
      eventResponder3_counter = 0;
      EventFiringClass efc;

      efc.testEvent1 += eventResponder3;
      efc.fireTestEvent1();
      REQUIRE(eventResponder3_counter == 1);
      
      // this isn't removing the pointer, but should be
      efc.testEvent1 -= eventResponder3;
      
      efc.fireTestEvent1();
      REQUIRE(eventResponder3_counter == 1);
    }
    WHEN("Removing middle one from a 3-long event list")
    {
      INFO("Phase 1");
      
      eventResponder3_counter = 0;
      eventResponder4_counter = 0;
      EventFiringClass efc;

      INFO("Phase 2");

      efc.testEvent1 += eventResponder4;
      efc.testEvent1 += eventResponder3;
      efc.testEvent1 += eventResponder4;
      efc.fireTestEvent1();
      REQUIRE(eventResponder4_counter == 2);
      REQUIRE(eventResponder3_counter == 1);
    
      INFO("Phase 3");
      
      efc.testEvent1 -= eventResponder3;
      
      INFO("Phase 4");
      
      efc.fireTestEvent1();
      REQUIRE(eventResponder4_counter == 4);
      REQUIRE(eventResponder3_counter == 1);
    }
    GIVEN("Experimental event code")
    {
      
    }
  }
}
