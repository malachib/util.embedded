#pragma once

#include "Menu.h"
#include "string_convert.h"

namespace FactUtilEmbedded
{
  template <class TIn, class TOut>
  class MenuFunction : public MenuCommand
  {
  protected:
    typedef TOut (*_handler)(TIn);

    const _handler handler;

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

  /*
   I still need to learn more about template specialization
  template<> class MenuFunction<void, int> : public MenuFunction<void, int>
  {
  protected:
    virtual void handleCommand(IMenu::Parameters p) override
    {
      handler(0);
    }
  }; */
}
