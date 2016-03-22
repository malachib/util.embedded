template <class TIn>
class ParameterClass_1
{
  TIn param1;
};

template <class TIn1, class TIn2>
class ParameterClass_2 : public ParameterClass_1<TIn1>
{
  TIn2 param2;
};

template <class TIn1, class TIn2, class TIn3>
class ParameterClass_3 : public ParameterClass_2<TIn1, TIn2>
{
  TIn3 param3;
};


template <class TOut>
class ParameterReturnClass
{
  TOut returnValue;
};
