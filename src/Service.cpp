#include "Service.h"
#include "fact/lib.h"

namespace FactUtilEmbedded
{

const char LightweightService::genericError[] PROGMEM = "Init failure";
const char LightweightService::emptyString[] PROGMEM = "";

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
    //state = Error;
    return false;
  }

  return start(initFunc);
}


bool LightweightService::start(initErrorStatus2 initFunc, LightweightService* dependsOn)
{
  if(!awaitDependency(dependsOn))
  {
    //state = Error;
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
    //state = Error;
    return false;
  }

  return start(initFunc);
}



bool LightweightService::awaitDependency(LightweightService* dependsOn)
{
#ifdef DEBUG
  cout.println(F("awaitDependency"));
#endif

  if(dependsOn)
  {
    setState(Waiting);

    const auto timeout = 10000;
    const uint32_t timeout_absolute = millis() + timeout;

    // TODO: Inified overflow watcher.  Old comment, not sure how to
    // do that..
    while(dependsOn->getState() == Unstarted
      || dependsOn->getState() == Starting
      || dependsOn->getState() == Waiting)
    {
      yield();
      if(millis() > timeout_absolute)
      {
        statusMessage = F("Timeout on dependency");
        return false;
      }
    }

    if(dependsOn->getState() == Error)
    {
      statusMessage = F("Dependency failure");
      return false;
    }
  }

  return true;
}

const __FlashStringHelper* LightweightService::getStateString() const
{
  switch(state)
  {
    case Unstarted: return F(SERVICE_STATUS_UNSTARTED);
    case Starting: return F(SERVICE_STATUS_STARTING);
    case Started: return F(SERVICE_STATUS_STARTED);
    case Error: return F(SERVICE_STATUS_ERROR);
    case Waiting: return F(SERVICE_STATUS_WAITING);
#ifdef FACT_LIB_STRICT
    default: return F("Unknown");
#endif
  }
}

void Service::restart(startService1 startFunc)
{
  LightweightService::setStatusMessage(NULL);
  setState(Starting);
  if(startFunc(*this))
  {
    // reset status message to NULL without firing event, since setState will
    // getStatusMessage() will auto-call getStatus() when statusMessage == NULL
    LightweightService::setStatusMessage(NULL);
    setState(Started);
  }
  else
    // when in error, leave status message alone because likely it contains
    // last thing attempted and failed at
    setState(Error);
}

void Service::start(startService1 startFunc, LightweightService* dependsOn)
{
  if(!awaitDependency(dependsOn))
  {
    LightweightService::setStatusMessage(NULL);
    //setState(Error);
    return;
  }

#ifdef SERVICE_FEATURE_RETAINED_DEPENDENCY
  this->dependsOn = dependsOn;
#endif

  start(startFunc);
}

#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC
bool Service::start()
{
  LightweightService::setStatusMessage(NULL);
  setState(Starting);
  if(!startFunc(*this))
  {
    setState(Error);
    return false;
  }
  else
  {
    LightweightService::setStatusMessage(NULL);
    setState(Started);
    return true;
  }

}

void Service::restart()
{
  restart(startFunc);
}
#endif
}
