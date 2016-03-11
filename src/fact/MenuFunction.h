#pragma once

#include "Menu.h"
#include "string_convert.h"

namespace FactUtilEmbedded
{
  template <class TIn>
  int invoke(int (*invokee)(TIn), IMenu::Parameters p)
  {
    TIn in1 = fromString<TIn>(p.parameters[0]);
    int result = invokee(in1);
    return result;
  }

  template <class TIn1, class TIn2>
  int invoke(int (*invokee)(TIn1, TIn2), IMenu::Parameters p)
  {
    auto in1 = fromString<TIn1>(p.parameters[0]);
    auto in2 = fromString<TIn2>(p.parameters[1]);
    int result = invokee(in1, in2);
    return result;
  }

  class MenuFunctionBase : public MenuCommand
  {
  };

  template <class TIn, class TOut>
  class MenuFunction : public MenuFunctionBase
  {
  protected:
    typedef TOut (*_handler)(TIn);

    const _handler handler;

  protected:
    virtual void handleCommand(Parameters p) override;

  public:
    MenuFunction(_handler handler) : handler(handler) {}
  };

  template <class TIn, class TOut>
  void MenuFunction<TIn, TOut>::handleCommand(IMenu::Parameters p)
  {
    TIn in1 = fromString<TIn>(p.parameters[0]);
    TOut result = handler(in1);
  }

  template <class TIn>
  class MenuFunctionNoOut : public MenuCommand
  {
  protected:
    typedef void (*_handler)(TIn);

    const _handler handler;

    virtual void handleCommand(Parameters p) override;

  public:
    MenuFunctionNoOut(_handler handler) : handler(handler) {}
  };

  /*
  template<> class MenuFunction<void, int> : public MenuFunction<void, int>
  {
  protected:
    virtual void handleCommand(IMenu::Parameters p) override
    {
      handler(0);
    }
  public:
    //MenuFunction(_handler)
  };
  */
}
