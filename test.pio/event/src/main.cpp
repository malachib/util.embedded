#include <Arduino.h>
#include <EventManager.h>

#include "features.h"

class EventFiringClass
{
  LOCAL_EVENT(EventFiringClass);
public:

  Event testEvent1;
} efc;

EventExp1<EventFiringClass*> eventExp1;


uint8_t eventResponder_counter = 0;

void eventResponder(EventFiringClass* source)
{
  eventResponder_counter++;
}


void setup() 
{
  efc.testEvent1 += eventResponder;
  //eventExp1 += eventResponder;
}

void loop()
{
  
}
