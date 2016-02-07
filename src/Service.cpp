#include "Service.h"

bool Service::init(const __FlashStringHelper*  (*initFunc)())
{
  state = Initializing;
  statusMessage = initFunc();
  if(statusMessage)
    state = Error;
  else
    state = Initialized;
}

bool Service::init(const __FlashStringHelper*  (*initFunc)(const __FlashStringHelper** subStatus))
{
  state = Initializing;
  statusMessage = initFunc(&statusMessage);
  if(statusMessage)
    state = Error;
  else
    state = Initialized;
}

const __FlashStringHelper* Service::getStatus()
{
  switch(state)
  {
    case Unstarted: return F("Waiting to initialize");
    case Initializing: return F("Initializing");
    case Initialized: return F("Initialized");
    case Error: return F("Error");
  }
}
