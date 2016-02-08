#include "Service.h"

const char Service::genericError[] PROGMEM = "Unable to initialize";

bool Service::start(initErrorStatus initFunc)
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

bool Service::start(initErrorStatus initFunc, Service* dependsOn)
{
  if(!awaitDependency(dependsOn))
  {
    state = Error;
    return false;
  }

  return start(initFunc);
}

bool Service::start(initFullStatus initFunc)
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


bool Service::start(initFullStatus initFunc, Service* dependsOn)
{
  if(!awaitDependency(dependsOn))
  {
    state = Error;
    return false;
  }

  return start(initFunc);
}



bool Service::awaitDependency(Service* dependsOn)
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
      statusMessage = F("Cannot initialize due to dependency failure");
      return false;
    }
  }

  return true;
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
