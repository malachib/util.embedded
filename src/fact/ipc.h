#pragma once

// NOTE: function references are available, but instance-member references are not
// (only instance-member pointers)
// I decided to use function references *when we can* to make consuming code more
// readable.  For why C++ doesn't have instance-member references:
//
// http://stackoverflow.com/questions/21952386/why-doesnt-reference-to-member-exist-in-c
//
// note also: TOut usage may not be compatible with ESP8266 compiler
#include "lib.h"
#include "../Console.h"

namespace FactUtilEmbedded
{
class ParameterClass_0
{
public:
  template <class TOut>
  TOut invoke(TOut (&func)())
  {
    return func();
  }
  
  void debugPrint() const {}
};

template <class TIn>
class ParameterClass_1
{
protected:
public: // FIX: temporarily making these public as we iron out architecture & POC
    TIn param1;

public:
  template <class TOut>
  TOut invoke(TOut (&func)(TIn))
  {
    return func(param1);
  }

  // TClass and TIn1 must always match
  template <class TClass, class TOut>
  TOut invoke(TOut (TClass::*func)())
  {
    return ((*param1).*func)();
  }
  
  void debugPrint() const
  {
    cout << F("p1: ") << param1;
    cout.println();
  }
};

template <class TIn1, class TIn2>
class ParameterClass_2 : public ParameterClass_1<TIn1>
{
protected:
public: // FIX: temporarily making these public as we iron out architecture & POC
  TIn2 param2;

public:
  template <class TOut>
  TOut invoke(TOut (&func)(TIn1, TIn2))
  {
    return func(ParameterClass_1<TIn1>::param1, param2);
  }


  // TClass and TIn1 must always match
  template <class TClass, class TOut>
  TOut invoke(TOut (TClass::*func)(TIn2))
  {
    return ((*ParameterClass_1<TIn1>::param1).*func)(param2);
  }

  void debugPrint() const
  {
    ParameterClass_1<TIn1>::debugPrint();
    cout << F("p2: ") << param2;
    cout.println();
  }
};

template <class TIn1, class TIn2, class TIn3>
class ParameterClass_3 : public ParameterClass_2<TIn1, TIn2>
{
protected:
public: // FIX: temporarily making these public as we iron out architecture & POC
  TIn3 param3;

public:
  template <class TOut>
  TOut invoke(TOut (&func)(TIn1, TIn2, TIn3))
  {
    return func(ParameterClass_1<TIn1>::param1, ParameterClass_2<TIn1, TIn2>::param2, param3);
  }
};


template <class TOut>
class ParameterReturnClass
{
  TOut returnValue;
};

class IInvoker
{
public:
  /* this doesn't work
  template <class TOut>
  virtual TOut invoke() = 0; */

  virtual void invoke() = 0;

  virtual void debugPrint() const = 0;
};


class IPCHelper;

template <class TParameters, class TFunc>
class CallHolder : public IInvoker
{
  friend IPCHelper;

protected:
  const TFunc func;

public:
  TParameters parameters;

  CallHolder(TFunc func) : func(func) {}

  virtual void invoke() override
  {
    parameters.invoke(func);
  }
  
  virtual void debugPrint() const override
  {
    parameters.debugPrint();
#ifdef UNIT_TEST
    //cout << F("Func ptr: ") << (uint32_t)(long)func;
#endif
    cout.println();
  }
};


class IPCHelper
{
public:
  // ditching template class TFunc trick because:
  // a) it doesn't gracefully resolve func ref vs func ptr
  // b) it forces TIn resolution to hang primarily off of create parameters
  //    vs actual function parameters, and we'd prefer function parameters

/*
  template <class TFunc, class TIn1>
  static CallHolder<ParameterClass_1<TIn1>, TFunc> _create(TFunc func, TIn1 in1)
  {
    auto m = createCallHolder(func);
    m.parameters.param1 = in1;
    return m;
  }*/


  template <class TOut>
  static CallHolder<ParameterClass_0, TOut (&)()> create(TOut (&func)())
  {
    CallHolder<ParameterClass_0, TOut (&)()> m(func);
    return m;
  }

  template <class TOut, class TIn1>
  static CallHolder<ParameterClass_1<TIn1>, TOut (&)(TIn1)> create(TOut (&func)(TIn1), TIn1 in1)
  {
    CallHolder<ParameterClass_1<TIn1>, TOut (&)(TIn1)> m(func);

    m.parameters.param1 = in1;
    return m;
  }

  template <class TOut, class TClass>
  static CallHolder<ParameterClass_1<TClass*>, TOut (TClass::*)()> create(TOut (TClass::*func)(), TClass* in1)
  {
    CallHolder<ParameterClass_1<TClass*>, TOut (TClass::*)()> m(func);

    m.parameters.param1 = in1;
    return m;
  }

  template <class TOut, class TIn1, class TIn2>
  static CallHolder<ParameterClass_2<TIn1, TIn2>, TOut (&)(TIn1, TIn2)>
    create(TOut (&func)(TIn1, TIn2), TIn1 in1, TIn2 in2)
  {
    CallHolder<ParameterClass_2<TIn1, TIn2>, TOut (&)(TIn1, TIn2)> m(func);

    m.parameters.param1 = in1;
    m.parameters.param2 = in2;
    return m;
  }

  template <class TOut, class TClass, class TIn2>
  static CallHolder<ParameterClass_2<TClass*, TIn2>, TOut (TClass::*)(TIn2)>
    create(TOut (TClass::*func)(TIn2), TClass* in1, TIn2 in2)
  {
    CallHolder<ParameterClass_2<TClass*, TIn2>, TOut (TClass::*)(TIn2)> m(func);

    m.parameters.param1 = in1;
    m.parameters.param2 = in2;
    return m;
  }
};

// NOTE: we probably want elementSize of 2 or 4 byte increments for alignment issues
template <uint8_t elementSize, uint8_t bufferSize>
class CallQueue 
{
  union Element
  {
    uint8_t buffer[elementSize];
  };
  
  Element elements[bufferSize];
  CircularBuffer<Element> queue;
  
public:
  CallQueue() : queue(elements, bufferSize) {}
  
  void put(IInvoker* invoker)
  {
    // dangerous *and* slow code, copies elementSize bytes from wherever invoker is
    // pointing
    queue.put((Element*)invoker);
  }
  
  IInvoker* get()
  {
    const Element& invoker = queue.get();
    
    return (IInvoker*)&invoker;
  }
};

}
