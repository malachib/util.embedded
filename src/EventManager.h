#pragma once

#include <Arduino.h>
// FIX: NODUINO is clunky check to ensure we grab our own uint8_t & friends
// however really we should try to grab it from stdint.h
#ifdef NODUINO
#include "fact/lib.h"
#endif
#include "fact/features.h"

// Thank you Mikael Patel for inspiration https://github.com/mikaelpatel/Arduino-Scheduler/blob/master/Scheduler/Queue.h


typedef void (*eventCallback)(void* parameter);

#ifndef HANDLEMANAGER_CAPACITY
#ifdef MEMORY_OPT_DATA
#define HANDLEMANAGER_CAPACITY 10
#else
#define HANDLEMANAGER_CAPACITY 20
#endif
#endif

class HandleManager
{
public:
  static const uint8_t nullHandle = 0;
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
  // initializes a new handle list
  handle init(void* data);
  // appends a handle to an existing handle list
  handle add(handle handle, void* data);
  void remove(handle handle);
  Handle* getHandle(handle handle) { return &handles[handle - 1]; }
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
    return HandleManager::add(event, (void*) callback);
  }
  void removeEvent(handle handle);
};

extern EventManager eventManager;


class HandleBase
{
protected:
  HandleManager::handle handle = HandleManager::nullHandle;

  void add(HandleManager* manager, void* data);
};


// be sure sizeof(T) == sizeof(void*)
template <class T>
class Event : public HandleBase
{
public:
  void add(void (*callback)(T parameter))
  {
    HandleBase::add(&eventManager, (void*)callback);
  }

  Event& operator+=(void (*callback)(T parameter))
  {
    add(callback);
    return *this;
  }

  void invoke(T parameter)
  {
    eventManager.invoke(handle, (void*) parameter);
  }

  Event& operator()(T parameter)
  {
    invoke(parameter);
    return *this;
  }
};


// wrapper class to more comfortably expose raw event class without
// exposing invoke() ability
template <class T>
class EventWrapper
{
  Event<T> events;

protected:
  // protected constructor makes re-assigning  more difficult
  // (which is what we want)
  EventWrapper() {}

  void invoke(T parameter)
  {
    events.invoke(parameter);
  }

  EventWrapper operator()(T parameter)
  {
    invoke(parameter);
    return *this;
  }

public:
  operator Event<T>&() const
  {
    return events;
  }

  EventWrapper& operator+=(void (*callback)(T parameter))
  {
    events += callback;
    return *this;
  }
};


#define LOCAL_EVENT(T) class Event : public EventWrapper<T*> { friend T; }

template <class T>
class PropertyWithEvents
{
  LOCAL_EVENT(PropertyWithEvents);

  T value;

protected:
  void setValue(T value)
  {
    this->value = value;
#ifdef SERVICE_FEATURE_EVENTS
    updated.invoke(this);
#endif
  }

public:
#ifdef SERVICE_FEATURE_EVENTS
  // fired when state or status message changes
  Event updated;
#endif

  PropertyWithEvents(T value) : value(value) {}
  PropertyWithEvents() {}

  T getValue() { return value; }
  operator T() { return getValue(); }

  PropertyWithEvents<T>& operator = (PropertyWithEvents<T> source)
  {
    setValue(source);
    return *this;
  }


  void addUpdatedEvent(void (*callback)(PropertyWithEvents* parameter))
  {
#ifdef SERVICE_FEATURE_EVENTS
    updated += callback;
#endif
  }
};


class PSTR_Property : public PropertyWithEvents<const __FlashStringHelper*> {};
class STR_Property : public PropertyWithEvents<const char*>
{
public:
  STR_Property() {}
  STR_Property(const char* value) : PropertyWithEvents(value) {}
};

class PubSTR_Property : public STR_Property
{
public:
  PubSTR_Property() {}
  PubSTR_Property(const char* value) : STR_Property(value) {}
  void setValue(const char* value) { STR_Property::setValue(value); }
};
