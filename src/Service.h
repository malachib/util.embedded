#pragma once

#include <Arduino.h>
#include <EventManager.h>

#define SERVICE_ERROR ((const __FlashStringHelper*) LightweightService::genericError)

typedef void (*initBasic)(void);
typedef const __FlashStringHelper* (*initErrorStatus)(void);
typedef const bool (*initErrorStatus2)(const __FlashStringHelper** status);
typedef const __FlashStringHelper* (*initFullStatus)(const __FlashStringHelper** status);

class Service;

typedef bool (*startService1)(Service& service);
typedef bool (*startService2);

class Named
{
  const __FlashStringHelper* name;

protected:
  Named() {}
  Named(const __FlashStringHelper* name) : name(name) {}

  void setName(const __FlashStringHelper* name) { this->name = name; }
public:
  const __FlashStringHelper* getName() { return name; }
};

// aka lightweight service
// TODO: consider begin/end paradigm instead of start/stop , since Arduino world prefers that
// TODO: refactor state machine and make Service into one --
class LightweightService
{
protected:
  enum State : uint8_t
  {
    Unstarted = 0,
    Starting = 1,
    Started = 2,
    Error = 3
  };

  void setState(State state) { this->state = state; }
  State getState() { return state; }

  // true = all dependencies satisfied, false = dependency did't initiailze
  bool awaitDependency(LightweightService* dependsOn);

private:
  State state;
  const __FlashStringHelper* statusMessage;

protected:
  void setStatusMessage(const __FlashStringHelper* statusMessage)
  {
    this->statusMessage = statusMessage;
  }

public:
  bool start(initErrorStatus initFunc);
  bool start(initErrorStatus initFunc, LightweightService* dependsOn);
  bool start(initErrorStatus2 initFunc);
  bool start(initErrorStatus2 initFunc, LightweightService* dependsOn);
  bool start(initFullStatus initFunc);
  bool start(initFullStatus initFunc, LightweightService* dependsOn);
  bool start(initBasic initFunc)
  {
    state = Starting;
    initFunc();
    state = Started;
  }

  const __FlashStringHelper* getStatus();
  const __FlashStringHelper* getStatusMessage() { return statusMessage; }

  static const char genericError[] PROGMEM;

  bool isInitialized() { return getState() == Started; }
};

class Service : public LightweightService, public Named
{
protected:
  void setState(State state)
  {
    LightweightService::setState(state);
#ifdef SERVICE_FEATURE_EVENTS
    statusUpdated.invoke(this);
#endif
  }

#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC
  startService1 startFunc;
#endif

public:
#ifdef SERVICE_FEATURE_EVENTS
  // fired when state or status message changes
  Event<Service*> statusUpdated;
#endif

  void start(const __FlashStringHelper* name, startService1);
  void start(const __FlashStringHelper* name, startService2);
  void restart(startService1);
#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC
  void restart();
#endif

  //void restart(startService1) { }

  // TODO: iron out fact that these shouldn't be public (use private / protected)
  // base class
  void setStatusMessage(const __FlashStringHelper* statusMessage)
  {
    LightweightService::setStatusMessage(statusMessage);
#ifdef SERVICE_FEATURE_EVENTS
    statusUpdated.invoke(this);
#endif
  }
};

class IService
{
protected:
  virtual void start() = 0;
  virtual void stop() {};
  virtual void pause() {};
};

class RestartableService : public Service
{

};

inline Print& operator <<(Print& p, LightweightService& arg)
{
  p.print(arg.getStatus());
  if(arg.getStatusMessage() != NULL)
  {
    p.print(F(": "));
    p.print(arg.getStatusMessage());
  }
  return p;
}
