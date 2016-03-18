#pragma once

#include "Menu.h"
#include "string_convert.h"

namespace FactUtilEmbedded
{
  /* FIX: gets a multiple declaration error in certain situations */
  // NOTE: works fine on atmega328p
#ifndef ESP8266
  void invoke(void (*invokee)(void), IMenu::Parameters p)
  {
    invokee();
  }
#endif

  template <class TIn, class TOut>
  TOut invoke(TOut (*invokee)(TIn), IMenu::Parameters p)
  {
    TIn in1 = fromString<TIn>(p.parameters[0]);
    TOut result = invokee(in1);
    return result;
  }

  template <class TIn>
  void invoke(void (*invokee)(TIn), IMenu::Parameters p)
  {
    TIn in1 = fromString<TIn>(p.parameters[0]);
    invokee(in1);
  }

  template <class TIn1, class TIn2>
  int invoke(int (*invokee)(TIn1, TIn2), IMenu::Parameters p)
  {
    auto in1 = fromString<TIn1>(p.parameters[0]);
    auto in2 = fromString<TIn2>(p.parameters[1]);
    int result = invokee(in1, in2);
    return result;
  }

  template <class TIn1, class TIn2>
  void invoke(void (*invokee)(TIn1, TIn2), IMenu::Parameters p)
  {
    auto in1 = fromString<TIn1>(p.parameters[0]);
    auto in2 = fromString<TIn2>(p.parameters[1]);
    invokee(in1, in2);
  }
}
