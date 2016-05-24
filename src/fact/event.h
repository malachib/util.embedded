#pragma once

#include "../EventManager.h"

namespace FactUtilEmbedded
{
  template <class TEvent>
  TEvent& operator+= (TEvent& event, typename TEvent::ParameterClass::stub func)
  {
    event.add((void*)func);
    return event;
  }

  template <class TEvent>
  TEvent& operator-= (TEvent& event, typename TEvent::ParameterClass::stub func)
  {
    event.remove((void*)func);
    return event;
  }

  class EventBase : public HandleBase
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
    void invoke(const TParameterClass& p) const
    {
      // EVENT_FEATURE_TYPE2 brute forces all Event0-Event3 invokes to route
      // through Type 2 mechanism.  Not necessarily better, just different
#ifdef EVENT_FEATURE_TYPE2
      eventManager.invokeType2(handleBase::handle, p);
#else
      eventManager.invokeType1(HandleBase::handle, p);
#endif
    }

    // invoke type 2 generates more up front code, but less code per invoke call
    // Use this if using event code more heavily -
    // NOTE: has an extra function pointer layer, so will run very slightly slower
    template <class TParameterClass>
    void invokeType2(const TParameterClass& p) const
    {
      eventManager.invokeType2(HandleBase::handle, p);
    }
  };
  
  namespace events
  {
    template <class ... Arguments>
    class Event
    {
    };
    
    template <>
    class Event<> : public EventBase
    {
    public:
      typedef FactUtilEmbedded::rpc::ParameterClass_0 ParameterClass;

      Event& operator()()
      {
        ParameterClass p;
        invoke(p);
        return *this;
      }
    };
    
    template <class TIn1>
    class Event<TIn1> : public EventBase
    {
    public:
      typedef FactUtilEmbedded::rpc::ParameterClass_1<TIn1> ParameterClass;

      Event& operator()(TIn1 in1)
      {
        ParameterClass p(in1);
        invoke(p);
        return *this;
      }

      void invokeT2(TIn1 in1) const
      {
        ParameterClass p(in1);
        invokeType2(p);
      }
      
    };

    template <class TIn1, class TIn2>
    class Event<TIn1, TIn2> : public EventBase
    {
    public:
      typedef FactUtilEmbedded::rpc::ParameterClass_2<TIn1, TIn2> ParameterClass;

      Event& operator()(TIn1 in1, TIn2 in2)
      {
        ParameterClass p(in1, in2);
        eventManager.invokeType1(handle, p);
        return *this;
      }
    };
  }

  class Event0 : public EventBase
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
  class Event1 : public EventBase
  {
  public:
    typedef FactUtilEmbedded::rpc::ParameterClass_1<TIn1> ParameterClass;

    Event1& operator()(TIn1 in1)
    {
      ParameterClass p(in1);
      invoke(p);
      return *this;
    }

    void invokeT2(TIn1 in1) const
    {
      ParameterClass p(in1);
      invokeType2(p);
    }
  };



  template <class TIn1, class TIn2>
  class Event2 : public EventBase
  {
  public:
    typedef FactUtilEmbedded::rpc::ParameterClass_2<TIn1, TIn2> ParameterClass;

    Event2& operator()(TIn1 in1, TIn2 in2)
    {
      ParameterClass p(in1, in2);
      eventManager.invokeType1(handle, p);
      return *this;
    }
  };
}
