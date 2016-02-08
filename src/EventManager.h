#include <Arduino.h>

// Thank you Mikael Patel for inspiration https://github.com/mikaelpatel/Arduino-Scheduler/blob/master/Scheduler/Queue.h


typedef void (*eventCallback)(void* parameter);

#ifndef HANDLEMANAGER_CAPACITY
#define HANDLEMANAGER_CAPACITY 20
#endif

class HandleManager
{
protected:
  static const int nullHandle = 255;
public:
  typedef uint8_t handle;

protected:
  class Handle
  {
    friend HandleManager;

  protected:
    void* data;
    handle next;
  public:
    void* getData() {return data;}
    handle getNext() {return next;}
  };

private:
  //Handle* handles;
  Handle handles[HANDLEMANAGER_CAPACITY];

protected:
  handle findFree();

public:
  handle add(handle handle, void* data);
  void remove(handle handle);
  Handle* getHandle(handle handle) { return &handles[handle]; }
};

//template <uint8_t NMEMB>
class EventManager : public HandleManager
{
  // NOTE: be sure to not change the memory size from Handle
  class Event : Handle
  {
  public:
    eventCallback getCallback() { return (eventCallback) data; }
    handle getNext() { return next; }
  };

  //Event allEvents[20];

  Event* getEvent(handle event) { return (Event*) getHandle(event); }

public:
  void invoke(handle event, void* parameter);
  handle addEvent(handle event, eventCallback callback)
  {
    HandleManager::add(event, (void*) callback);
  }
  void removeEvent(handle handle);
};
