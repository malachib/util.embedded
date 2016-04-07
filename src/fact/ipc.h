// NOTE: function references are available, but instance-member references are not
// (only instance-member pointers)
// I decided to use function references *when we can* to make consuming code more
// readable.  For why C++ doesn't have instance-member references:
//
// http://stackoverflow.com/questions/21952386/why-doesnt-reference-to-member-exist-in-c
//
// note also: TOut usage may not be compatible with ESP8266 compiler

class ParameterClass_0
{
public:
  template <class TOut>
  TOut invoke(TOut (&func)())
  {
    return func();
  }
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
};


class IPCHelper;

template <class TParameters, class TFunc>
class IPCMessage : public IInvoker
{
  friend IPCHelper;

protected:
  const TFunc func;

public:
  TParameters parameters;

  IPCMessage(TFunc func) : func(func) {}

  virtual void invoke() override
  {
    parameters.invoke(func);
  }
};

template <class TParameters, class TFunc>
class IPCMessageMethod : IPCMessage<TParameters, TFunc>
{
  void invoke()
  {
  }
};

//template<class TOut> TOut createIPCMessage(_test_func1);

//template<>
template <class TOut>
IPCMessage<ParameterClass_0, TOut (&)()> createIPCMessage(TOut (&func)())
{
  IPCMessage<ParameterClass_0, TOut (&)()> m(func);
  return m;
}

template<class TOut, class TIn>
IPCMessage<ParameterClass_1<TIn>, TOut (&)(TIn)> createIPCMessage(TOut (&func)(TIn))
{
  IPCMessage<ParameterClass_1<TIn>, TOut (&)(TIn)> m(func);
  return m;
  //m.parameters.param1 =
}


template<class TOut, class TIn1, class TIn2>
IPCMessage<ParameterClass_2<TIn1, TIn2>, TOut (&)(TIn1, TIn2)> createIPCMessage(TOut (&func)(TIn1, TIn2))
{
  IPCMessage<ParameterClass_2<TIn1, TIn2>, TOut (&)(TIn1, TIn2)> m(func);
  return m;
  //m.parameters.param1 =
}


template <class TOut, class T>
IPCMessage<ParameterClass_1<T*>, TOut (T::*)()> createIPCMessage(TOut (T::*func)())
{
  IPCMessage<ParameterClass_1<T*>, TOut (T::*)()> m(func);
  return m;
}


template <class TOut, class T, class TIn1>
IPCMessage<ParameterClass_2<T*, TIn1>, TOut (T::*)(TIn1)> createIPCMessage(TOut (T::*func)(TIn1))
{
  IPCMessage<ParameterClass_2<T*, TIn1>, TOut (T::*)(TIn1)> m(func);
  return m;
}

class IPCHelper
{
public:
  // ditching template class TFunc trick because:
  // a) it doesn't gracefully resolve func ref vs func ptr
  // b) it forces TIn resolution to hang primarily off of create parameters
  //    vs actual function parameters, and we'd prefer function parameters

/*
  template <class TFunc, class TIn1>
  static IPCMessage<ParameterClass_1<TIn1>, TFunc> _create(TFunc func, TIn1 in1)
  {
    auto m = createIPCMessage(func);
    m.parameters.param1 = in1;
    return m;
  }*/


  template <class TOut>
  static IPCMessage<ParameterClass_0, TOut (&)()> create(TOut (&func)())
  {
    return createIPCMessage(func);
  }

  template <class TOut, class TIn1>
  static IPCMessage<ParameterClass_1<TIn1>, TOut (&)(TIn1)> create(TOut (&func)(TIn1), TIn1 in1)
  {
    auto m = createIPCMessage(func);
    m.parameters.param1 = in1;
    return m;
  }

  template <class TOut, class TClass>
  static IPCMessage<ParameterClass_1<TClass*>, TOut (TClass::*)()> create(TOut (TClass::*func)(), TClass* in1)
  {
    auto m = createIPCMessage(func);
    m.parameters.param1 = in1;
    return m;
  }

  template <class TOut, class TIn1, class TIn2>
  static IPCMessage<ParameterClass_2<TIn1, TIn2>, TOut (&)(TIn1, TIn2)>
    create(TOut (&func)(TIn1, TIn2), TIn1 in1, TIn2 in2)
  {
    auto m = createIPCMessage(func);
    m.parameters.param1 = in1;
    m.parameters.param2 = in2;
    return m;
  }

  template <class TOut, class TClass, class TIn2>
  static IPCMessage<ParameterClass_2<TClass*, TIn2>, TOut (TClass::*)(TIn2)>
    create(TOut (TClass::*func)(TIn2), TClass* in1, TIn2 in2)
  {
    auto m = createIPCMessage(func);
    m.parameters.param1 = in1;
    m.parameters.param2 = in2;
    return m;
  }
};
