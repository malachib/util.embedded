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
    void* getData() const {return data;}
    handle getNext() const {return next;}
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

  static const Handle* const getHandleRO(const Handle* const handles, handle h)
  {
    return &handles[h - 1];
  }

  // helper for RO operations
  const Handle& getHandleNative(uint8_t h) const
  {
    return handles[h];
  }
public:
  // clears and initializes handle list overall
  void init();
  uint8_t available() const;

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
  const Handle* const getHandleRO(handle handle) const
  {
    return getHandleRO(handles, handle);
  }
  bool remove(handle startNode, void* data)
  {
    return remove(handles, getHandle(startNode), data);
  }
};

#ifdef EVENT_FEATURE_VA
typedef void (*eventCallback)(void* sender, va_list argp);
#else
typedef void (*eventCallback)(void* sender);
#endif

//template <uint8_t NMEMB>
class EventManager : public HandleManager
{
  // NOTE: be sure to not change the memory size from Handle,
  // so that handles map properly onto HandleManager::Handle
  class Event : public Handle
  {
  public:
    eventCallback getCallback() const { return (eventCallback) data; }
  };

  const Event* const getEvent(handle event) const
  { return (const Event* const ) getHandleRO(event); }

  typedef void (&_p_invoke)(const void* pc, const void* callback);

  // shim call to suppress mass template-code generation for many
  // different varieties of invoke call
  void invokeType2_helper(HandleManager::handle h,
    const _p_invoke p_invoke,
    const void* pc) const;

  // somewhat-safely convert Handle's void* data to the specific
  // TParameterClass::stub desired for the operation
  template <class TParameterClass>
  static typename TParameterClass::stub castCallback(const void* callback)
  {
    // very fragile code
    // note sure why but stub ends up being a function PTR not reference, even
    // though stubs are explicitly references
    auto safecast = (uint8_t*) callback;
    auto _callback = reinterpret_cast<typename TParameterClass::stub>(*safecast);

    return *_callback;
  }

  // this is a function helper to encapsulate template behavior, so that
  // we can reuse our loop function elsewhere to hopefully save code space
  // now what we need to do is to do some code size comparisons
  template <class TParameterClass>
  static void __p_invoke(const void* pc, const void* callback)
  {
    auto _pc = reinterpret_cast<const TParameterClass*>(pc);

    _pc->invoke(castCallback<TParameterClass>(callback));
  }
public:

#ifdef EVENT_FEATURE_VA
  void invoke(handle event, void* parameter, va_list argp);
#else
  void invoke(handle event, void* parameter);
#endif

  template <class TParameterClass>
  void invokeType1(handle h, const TParameterClass& p) const
  {
    while(h != nullHandle)
    {
      const Event* const event = getEvent(h);

      p.invoke(castCallback<TParameterClass>(event->getData()));
      h = event->getNext();
    }
  }

  template <class TParameterClass>
  void invokeType2(HandleManager::handle h, const TParameterClass& p) const
  {
    invokeType2_helper(h, __p_invoke<TParameterClass>, &p);
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


template <class TEvent>
TEvent& operator+= (TEvent& event, typename TEvent::ParameterClass::stub func)
{
  event.add((void*)func);
}

template <class TEvent>
TEvent& operator-= (TEvent& event, typename TEvent::ParameterClass::stub func)
{
  event.remove((void*)func);
}

class EventExp : public HandleBase
{
  template <class TEvent>
  friend TEvent& operator+= (TEvent& event, typename TEvent::ParameterClass::stub func);
  template <class TEvent>
  friend TEvent& operator-= (TEvent& event, typename TEvent::ParameterClass::stub func);

protected:
  void add(void* data)
  {
#ifdef UNIT_TEST
    //printf("data = %lx\r\n", data);
#endif
    HandleBase::add(&eventManager, data);
  }

  void remove(void* data)
  {
    HandleBase::remove(&eventManager, data);
  }

  // invoke type 1 generates a bit less up front code, but a bit more code
  // per call.  Use this if only lightly using event code
  template <class TParameterClass>
  void invoke(const TParameterClass& p)
  {
    eventManager.invokeType1(HandleBase::handle, p);
  }

  // invoke type 2 generates more up front code, but less code per invoke call
  // Use this if using event code more heavily -
  // NOTE: has an extra function pointer layer, so will run very slightly slower
  template <class TParameterClass>
  void invokeType2(const TParameterClass& p)
  {
    eventManager.invokeType2(HandleBase::handle, p);
  }
};


class Event0 : public EventExp
{
public:
  typedef FactUtilEmbedded::rpc::ParameterClass_0 ParameterClass;

  Event0& operator()()
  {
    ParameterClass p;
    invoke(p);
    return *this;
  }
};

template <class TIn1>
class Event1 : public EventExp
{
public:
  typedef FactUtilEmbedded::rpc::ParameterClass_1<TIn1> ParameterClass;

  Event1& operator()(TIn1 in1)
  {
    ParameterClass p(in1);
    invoke(p);
    return *this;
  }

  void invokeT2(TIn1 in1)
  {
    ParameterClass p(in1);
    invokeType2(p);
  }
};






template <class TIn1, class TIn2>
class Event2 : public EventExp
{
public:
  typedef FactUtilEmbedded::rpc::ParameterClass_2<TIn1, TIn2> ParameterClass;

  Event2& operator()(TIn1 in1, TIn2 in2)
  {
    ParameterClass p(in1, in2);
    eventManager.invokeType1(handle, p);
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

#ifdef EVENT_FEATURE_VA
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
#else
  void invoke(T parameter)
  {
    eventManager.invoke(handle, (void*) parameter);
  }

  Event& operator()(T parameter)
  {
    invoke(parameter);
    return *this;
  }
#endif

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
#ifdef EVENT_FEATURE_VA
    VA_WRAPPER(parameter);
    events._invoke(parameter, va.argp);
#else
    events.invoke(parameter);
#endif
  }

  EventWrapper operator()(T parameter ...)
  {
#ifdef EVENT_FEATURE_VA
    VA_WRAPPER(parameter);
    events._invoke(parameter, va.argp);
#else
    events.invoke(parameter);
#endif
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
