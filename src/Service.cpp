#include "Service.h"

const char LightweightService::genericError[] PROGMEM = "Init failure";

bool LightweightService::start(initErrorStatus initFunc)
{
  state = Initializing;
  statusMessage = initFunc();
  if(statusMessage)
  {
    state = Error;
    return false;
  }
  else
  {
    state = Initialized;
    return true;
  }
}


bool LightweightService::start(initErrorStatus2 initFunc)
{
  state = Initializing;
  if(initFunc(&statusMessage))
  {
    state = Initialized;
    return true;
  }
  else
  {
    state = Error;
    return false;
  }
}


bool LightweightService::start(initErrorStatus initFunc, LightweightService* dependsOn)
{
  if(!awaitDependency(dependsOn))
  {
    state = Error;
    return false;
  }

  return start(initFunc);
}


bool LightweightService::start(initErrorStatus2 initFunc, LightweightService* dependsOn)
{
  if(!awaitDependency(dependsOn))
  {
    state = Error;
    return false;
  }

  return start(initFunc);
}



bool LightweightService::start(initFullStatus initFunc)
{
  state = Initializing;
  statusMessage = initFunc(&statusMessage);
  if(statusMessage)
  {
    state = Error;
    return false;
  }
  else
  {
    state = Initialized;
    return true;
  }
}


bool LightweightService::start(initFullStatus initFunc, LightweightService* dependsOn)
{
  if(!awaitDependency(dependsOn))
  {
    state = Error;
    return false;
  }

  return start(initFunc);
}



bool LightweightService::awaitDependency(LightweightService* dependsOn)
{
  if(dependsOn)
  {
    // TODO: once I have a unified overflow watcher, do timeout logic here (20s)
    while(dependsOn->getState() == Unstarted || dependsOn->getState() == Initializing)
    {
      yield();
    }

    if(dependsOn->getState() == Error)
    {
      statusMessage = F("Dependency failure");
      return false;
    }
  }

  return true;
}

const __FlashStringHelper* LightweightService::getStatus()
{
  switch(state)
  {
    case Unstarted: return F("Waiting to initialize");
    case Initializing: return F("Initializing");
    case Initialized: return F("Initialized");
    case Error: return F("Error");
  }
}

void Service::start(const __FlashStringHelper* name, startService1 startFunc)
{
  setName(name);
  setState(Initializing);
  if(startFunc(*this))
    setState(Initialized);
  else
    setState(Error);
}
