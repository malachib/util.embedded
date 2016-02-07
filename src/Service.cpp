#include "Service.h"

bool Service::start(initErrorStatus initFunc)
{
  state = Initializing;
  statusMessage = initFunc();
  if(statusMessage)
    state = Error;
  else
    state = Initialized;
}

bool Service::start(initFullStatus initFunc)
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
