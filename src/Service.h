#pragma once

#include <Arduino.h>

typedef void (*initBasic)(void);
typedef const __FlashStringHelper* (*initErrorStatus)(void);
typedef const __FlashStringHelper* (*initFullStatus)(const __FlashStringHelper** status);

// aka lightweight service
// TODO: consider begin/end paradigm instead of start/stop , since Arduino world prefers that
class Service
{
protected:
  enum State : uint8_t
  {
    Unstarted = 0,
    Initializing = 1,
    Initialized = 2,
    Error = 3
  };

  void setState(State state) { this->state = state; }

private:
  State state;
  const __FlashStringHelper* statusMessage;

public:
  bool start(initErrorStatus initFunc);
  // FIX: figure out overloading better so that we can have a better name than 'init2'
  bool start(initFullStatus initFunc);
  bool start(initBasic initFunc)
  {
    state = Initializing;
    initFunc();
    state = Initialized;
  }

  const __FlashStringHelper* getStatus();
  const __FlashStringHelper* getStatusMessage() { return statusMessage; }
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

inline Print& operator <<(Print& p, Service& arg)
{
  p.print(arg.getStatus());
  if(arg.getStatusMessage() != NULL)
  {
    p.print(F(": "));
    p.print(arg.getStatusMessage());
  }
  return p;
}
