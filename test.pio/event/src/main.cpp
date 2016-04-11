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
#if TEST_DEPTH > 1
  Event testEvent3;
#endif
#if TEST_DEPTH > 2
#endif
#endif
#if defined (FEATURE_EVENT_TEMPLATE1) || defined (FEATURE_EVENT_TEMPLATE2)
  Event1<EventFiringClass*> testEvent1;
#if TEST_DEPTH > 0
  Event1<EventFiringClass*> testEvent2;
#endif
#if TEST_DEPTH > 1
  Event1<EventFiringClass*> testEvent3;
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
#if TEST_DEPTH > 1
    testEvent3(this);
#endif
#else
    testEvent1.invokeT2(this);
#if TEST_DEPTH > 0
    testEvent2.invokeT2(this);
#endif
#if TEST_DEPTH > 1
    testEvent3.invokeT2(this);
#endif
#endif
#endif
  }
} efc;


#if TEST_DEPTH > 2
class EventFiringClass2
{
  LOCAL_EVENT(EventFiringClass2);

public:
#if defined (FEATURE_EVENT_ORIGINAL)
  Event testEvent1;
#else
  Event1<EventFiringClass2*> testEvent1;
#endif

  void fireEvent1()
  {
#ifdef FEATURE_EVENT
#ifndef FEATURE_EVENT_TEMPLATE2
    testEvent1(this);
#else
    testEvent1.invokeT2(this);
#endif
#endif
  }
} efc2;
#endif

uint8_t eventResponder_counter = 0;

void eventResponder(EventFiringClass* source)
{
  eventResponder_counter++;
}

#if TEST_DEPTH > 2
void eventResponder2(EventFiringClass2* source)
{
  eventResponder_counter++;
}
#endif

void setup()
{
#ifdef FEATURE_EVENT
  efc.testEvent1 += eventResponder;
#if TEST_DEPTH > 0
  efc.testEvent2 += eventResponder;
#endif
#if TEST_DEPTH > 1
  efc.testEvent3 += eventResponder;
#endif
#if TEST_DEPTH > 2
  efc2.testEvent1 += eventResponder2;
#endif
#endif
}

void loop()
{
  efc.fireEvent1();
#if TEST_DEPTH > 2
  efc2.fireEvent1();
#endif

  delay(1000);
}
