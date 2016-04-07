
class ParameterClass_0
{
public:
  template <class TOut>
  TOut invoke(TOut (*func)())
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
  TOut invoke(TOut (*func)(TIn))
  {
    return func(param1);
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
  TOut invoke(TOut (*func)(TIn1, TIn2))
  {
    return func(ParameterClass_1<TIn1>::param1, param2);
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
  TOut invoke(TOut (*func)(TIn1, TIn2, TIn3))
  {
    return func(ParameterClass_1<TIn1>::param1, ParameterClass_2<TIn1, TIn2>::param2, param3);
  }
};


template <class TOut>
class ParameterReturnClass
{
  TOut returnValue;
};


template <class TParameters, class TFunc>
class IPCMessage
{
protected:
  const TFunc func;

public:
  IPCMessage(TFunc func) : func(func) {}

  // FIX: temporarily making these public
  TParameters parameters;

  void invoke()
  {
    parameters.invoke(func);
  }
};

class IPCHelper
{
public:

  //IPCMessage
};


//template<class TOut> TOut createIPCMessage(_test_func1);

//template<>
IPCMessage<ParameterClass_0, void (*)()> createIPCMessage(void (*func)())
{
  IPCMessage<ParameterClass_0, void (*)()> m(func);
  return m;
}

template<class TIn>
IPCMessage<ParameterClass_1<TIn>, void (*)(TIn)> createIPCMessage(void (*func)(TIn))
{
  IPCMessage<ParameterClass_1<TIn>, void (*)(TIn)> m(func);
  return m;
  //m.parameters.param1 =
}


template<class TIn1, class TIn2>
IPCMessage<ParameterClass_2<TIn1, TIn2>, void (*)(TIn1, TIn2)> createIPCMessage(void (*func)(TIn1, TIn2))
{
  IPCMessage<ParameterClass_2<TIn1, TIn2>, void (*)(TIn1, TIn2)> m(func);
  return m;
  //m.parameters.param1 =
}
