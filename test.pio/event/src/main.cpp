#include <Arduino.h>
#include <EventManager.h>

#include "features.h"

class EventFiringClass
{
  LOCAL_EVENT(EventFiringClass);
public:


#if defined (FEATURE_EVENT_ORIGINAL)
  Event testEvent1;
#if TEST_DEPTH > 0
  Event testEvent2;
#endif
#endif
#if defined (FEATURE_EVENT_TEMPLATE1) || defined (FEATURE_EVENT_TEMPLATE2)
  Event1<EventFiringClass*> testEvent1;
#if TEST_DEPTH > 0
  Event1<EventFiringClass*> testEvent2;
#endif
#endif

  void fireEvent1()
  {
#ifdef FEATURE_EVENT
#ifndef FEATURE_EVENT_TEMPLATE2
    testEvent1(this);
#if TEST_DEPTH > 0
    testEvent2(this);
#endif
#else
    testEvent1.invokeT2(this);
#if TEST_DEPTH > 0
    testEvent2.invokeT2(this);
#endif
#endif
#endif
  }
} efc;

uint8_t eventResponder_counter = 0;

void eventResponder(EventFiringClass* source)
{
  eventResponder_counter++;
}


void setup()
{
#ifdef FEATURE_EVENT
  efc.testEvent1 += eventResponder;
#if TEST_DEPTH > 0
  efc.testEvent2 += eventResponder;
#endif
#endif
}

void loop()
{
  efc.fireEvent1();

  delay(1000);
}
