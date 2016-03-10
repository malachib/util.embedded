#pragma once

#include "Menu.h"

namespace FactUtilEmbedded
{
  template <class TIn>
  class StringConverter
  {

  };

  template<> class StringConverter<int>
  {
  public:
    int fromString(const char* input)
    {
      return atoi(input);
    }
  };

  template <class TIn, class TOut>
  class MenuFunction : public MenuCommand
  {
    typedef TOut (*_handler)(TIn);

    _handler handler;
  protected:
    virtual void handleCommand(Parameters p) override;

  public:
    MenuFunction(_handler handler) : handler(handler) {}
  };

  template <class TIn, class TOut>
  void MenuFunction<TIn, TOut>::handleCommand(IMenu::Parameters p)
  {
    TOut result = handler(0);
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
