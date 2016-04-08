#pragma once

// TODO: add a 2nd parameter, since the sender/this pointer can be weak by itself
// TODO: make wrappers so that this pointer is passed automatically
#include <Arduino.h>
// FIX: NODUINO is clunky check to ensure we grab our own uint8_t & friends
// however really we should try to grab it from stdint.h
#ifdef NODUINO
#include "fact/lib.h"
#endif
#include "fact/features.h"
#include "fact/rpc.h"

#include <stdarg.h>


#define VA_WRAPPER(p) \
    VAWrapper va;     \
    va_start(va.argp, p) \

class VAWrapper
{
public:
  va_list argp;

  // don't call this explicitly - use VA_WRAPPER macro
  VAWrapper()
  {
  }
  
  // because we can expect this to always inline out and not really use a ref,
  // this should always work
  operator va_list&() { return argp; }
  
  ~VAWrapper() { va_end(argp); }
};

// Thank you Mikael Patel for inspiration https://github.com/mikaelpatel/Arduino-Scheduler/blob/master/Scheduler/Queue.h


#ifndef HANDLEMANAGER_CAPACITY
#ifdef MEMORY_OPT_DATA
#define HANDLEMANAGER_CAPACITY 10
#else
#define HANDLEMANAGER_CAPACITY 20
#endif
#endif

class HandleBase;

// handle manager is like a set of linked lists coexisting in one pool
// a handle is a member of a particular list of handles.  Each handle has
// a singly-linked node pointing to the next handle until nullHandle is
// reached
class HandleManager
{
  friend HandleBase;
  
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
  // allocate a data handle
  static handle alloc(Handle* handles, uint8_t capacity, void* data)
  {
    // look for any free ones (somewhat CPU expensive, but we shouldn't be adding/removing)
    // events so often - mainly firing them
    for(uint8_t i = 0; i < capacity; i++)
    {
      Handle& hEval = handles[i];

      if(hEval.getData() == NULL)
      {
        hEval.data = data;
        hEval.next = nullHandle;
        return i + 1;
      }
    }

    return nullHandle;
  }

  // allocate a data handle
  handle alloc(void* data)
  {
    return alloc(handles, HANDLEMANAGER_CAPACITY, data);
  }

  // remove handle with "data" as its data from within a handle chain
  // (not the entire handle pool).  Returns false if startNode itself
  // needs to be removed (external remove must happen), otherwise 
  // true
  static bool remove(Handle* handles, Handle* startNode, void* data);

  static Handle* getHandle(Handle* handles, handle h)
  {
    return &handles[h - 1];
  }
  
public:
  // clears and initializes handle list overall
  void init();
  uint8_t available();

  // initializes a new handle list
  handle init(void* data);
  // appends a handle to an existing handle list
  handle add(handle handle, void* data);
  void remove(handle handle);
  // clears this and all subsequent handles in the
  // list.  note does not gauruntee any handles preceding
  // this handle in the list are cleared
  void clear(handle handle);
  Handle* getHandle(handle handle) { return getHandle(handles, handle); }
  bool remove(handle startNode, void* data)
  {
    return remove(handles, getHandle(startNode), data);
  }
};

typedef void (*eventCallback)(void* sender, va_list argp);

//template <uint8_t NMEMB>
class EventManager : public HandleManager
{
  // NOTE: be sure to not change the memory size from Handle, 
  // so that handles map properly onto HandleManager::Handle
  class Event : Handle
  {
  public:
    eventCallback getCallback() { return (eventCallback) data; }
    handle getNext() { return next; }
  };

  Event* getEvent(handle event) { return (Event*) getHandle(event); }

public:
  /*
  void invoke(handle event, void* parameter, ...)
  {
    VA_WRAPPER(parameter);
    //va_start(argp, parameter);
    invoke(event, parameter, va.argp);
    //va_end(argp);
  } */
  void invoke(handle event, void* parameter, va_list argp);
  
  template <class TParameterClass>
  void _invokeExp(handle h, TParameterClass p)
  {
    while(h != nullHandle)
    {
      Event* event = getEvent(h);
      
      auto callback = (TParameterClass::stub)(event->getCallback());
      
      p.invoke(callback);
      //p.
    }
  }
};

extern EventManager eventManager;


class HandleBase
{
protected:
  HandleManager::handle handle = HandleManager::nullHandle;

  void add(HandleManager* manager, void* data);
  void clear(HandleManager* manager)
  {
    manager->clear(handle);
    handle = HandleManager::nullHandle;
  }
  void remove(HandleManager* manager, void* data);
};


class EventExp : public HandleBase
{
protected:
  void add(void* data)
  {
    HandleBase::add(&eventManager, data);
  }
};

template <class TIn1>
class EventExp1 : public EventExp
{
  typedef FactUtilEmbedded::rpc::ParameterClass_1<TIn1> ParameterClass;
  typedef void (&stub)(TIn1);
  
public:
  EventExp1& operator()(TIn1 in1)
  {
    ParameterClass p(in1);
    eventManager._invokeExp(handle, p);
  }
  
  EventExp1& operator+=(stub callback)
  {
    add(callback);
    return *this;
  }
};


template <class TIn1, class TIn2>
class EventExp2 : public EventExp
{
public:
  EventExp2& operator()(TIn1 in1, TIn2 in2)
  {
    FactUtilEmbedded::rpc::ParameterClass_2<TIn1, TIn2> p(in1, in2);
    eventManager._invokeExp(handle, p);
  }
};


// be sure sizeof(T) == sizeof(void*)
template <class T>
class Event : public HandleBase
{
  typedef void _eventCallback(T);
  typedef void __eventCallback(T, va_list);

public:
  void add(_eventCallback callback)
  {
    HandleBase::add(&eventManager, (void*)callback);
  }

  void add(__eventCallback callback)
  {
    HandleBase::add(&eventManager, (void*)callback);
  }

  template <class TCallback>
  Event& operator+=(TCallback callback)
  {
    add(callback);
    return *this;
  }
  
  Event& operator-=(void (*callback)(T parameter))
  {
    HandleBase::remove(&eventManager, (void*)callback);
    return *this;
  }

  void _invoke(T parameter, va_list argp)
  {
    eventManager.invoke(handle, (void*) parameter, argp);
  }
  
  void invoke(T parameter ...)
  {
    VA_WRAPPER(parameter);
    _invoke(parameter, va);
  }

  Event& operator()(T parameter...)
  {
    VA_WRAPPER(parameter);
    _invoke(parameter, va);

    return *this;
  }
  
  template <class TIn1>
  void invokeExp(TIn1 in1)
  {
    FactUtilEmbedded::rpc::ParameterClass_1<TIn1> p(in1);
    eventManager._invokeExp(handle, p);
  }

  template <class TIn1, class TIn2>
  void invokeExp(TIn1 in1, TIn2 in2)
  {
    FactUtilEmbedded::rpc::ParameterClass_2<TIn1, TIn2> p(in1, in2);
    eventManager._invokeExp(handle, p);
  }

  void clear() { HandleBase::clear(&eventManager); }
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

  void invoke(T parameter ...)
  {
    VA_WRAPPER(parameter);
    events._invoke(parameter, va.argp);
  }

  EventWrapper operator()(T parameter ...)
  {
    VA_WRAPPER(parameter);
    events._invoke(parameter, va.argp);
    return *this;
  }

public:
  operator Event<T>&() const
  {
    return events;
  }

  template <class TCallback>
  EventWrapper& operator+=(TCallback callback)
  {
    events += callback;
    return *this;
  }
  
  template <class TCallback>
  EventWrapper& operator-=(TCallback callback)
  {
    events -= callback;
    return *this;
  }

  void clear() { events.clear(); }
};


#define LOCAL_EVENT(T) class Event : public EventWrapper<T*> { friend T; }
//#define DECLARE_EVENT(P) class : public EventWrapper<P*> { friend P; }

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
