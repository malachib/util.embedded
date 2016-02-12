#include "Service.h"
#include "lib.h"

namespace FactUtilEmbedded
{

const char LightweightService::genericError[] PROGMEM = "Init failure";

bool LightweightService::start(initErrorStatus initFunc)
{
  state = Starting;
  statusMessage = initFunc();
  if(statusMessage)
  {
    state = Error;
    return false;
  }
  else
  {
    state = Started;
    return true;
  }
}



bool LightweightService::start(initErrorStatus2 initFunc)
{
  state = Starting;
  if(initFunc(&statusMessage))
  {
    state = Started;
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
  state = Starting;
  statusMessage = initFunc(&statusMessage);
  if(statusMessage)
  {
    state = Error;
    return false;
  }
  else
  {
    state = Started;
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
#if DEBUG
  Serial.println(F("awaitDependency"));
#endif

  if(dependsOn)
  {
    // TODO: once I have a unified overflow watcher, do timeout logic here (20s)
    while(dependsOn->getState() == Unstarted || dependsOn->getState() == Starting)
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
#ifdef MEMORY_OPT_CODE
    case Unstarted: return F("Unstarted");
#else
    case Unstarted: return F("Waiting to start");
#endif
    case Starting: return F("Starting");
    case Started: return F("Started");
    case Error: return F("Error");
  }
}

void Service::restart(startService1 startFunc)
{
  setState(Starting);
  if(startFunc(*this))
  {
    // reset status message to NULL without firing event, since setState will
    LightweightService::setStatusMessage(NULL);
    setState(Started);
  }
  else
    setState(Error);
}

void Service::start(const __FlashStringHelper* name, startService1 startFunc)
{
  setName(name);
#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC
  this->startFunc = startFunc;
#endif
  restart(startFunc);
}

#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC
void Service::restart()
{
  restart(startFunc);
}
#endif
}
