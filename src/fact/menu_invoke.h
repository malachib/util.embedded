#pragma once

#include "Menu.h"
#include "string_convert.h"

namespace FactUtilEmbedded
{
  /* FIX: gets a multiple declaration error in certain situations */
  // NOTE: works fine on atmega328p
#ifndef ESP8266
  inline void invoke(void (*invokee)(void), IMenu::Parameters p)
  {
    invokee();
  }
#endif

  template <class TIn, class TOut>
  inline TOut invoke(TOut (*invokee)(TIn), IMenu::Parameters p)
  {
    TIn in1 = fromString<TIn>(p.parameters[0]);
    TOut result = invokee(in1);
    return result;
  }

  template <class TIn>
  inline void invoke(void (*invokee)(TIn), IMenu::Parameters p)
  {
    TIn in1 = fromString<TIn>(p.parameters[0]);
    invokee(in1);
  }

  template <class TIn1, class TIn2>
  inline int invoke(int (*invokee)(TIn1, TIn2), IMenu::Parameters p)
  {
    auto in1 = fromString<TIn1>(p.parameters[0]);
    auto in2 = fromString<TIn2>(p.parameters[1]);
    int result = invokee(in1, in2);
    return result;
  }

  template <class TIn1, class TIn2>
  inline void invoke(void (*invokee)(TIn1, TIn2), IMenu::Parameters p)
  {
    auto in1 = fromString<TIn1>(p.parameters[0]);
    auto in2 = fromString<TIn2>(p.parameters[1]);
    invokee(in1, in2);
  }


  template <class TOut>
  inline void validateInvokeNative(TOut (*invokee)(), IMenu::Parameters p)
  {
  }


  template <class TIn, class TOut>
  inline void validateInvokeNative(TOut (*invokee)(TIn), IMenu::Parameters p)
  {
    validateString<TIn>(p.parameters[0]);
  }


  template <class TIn1, class TIn2, class TOut>
  inline void validateInvokeNative(TOut (*invokee)(TIn1, TIn2), IMenu::Parameters p)
  {
    validateString<TIn1>(p.parameters[0]);
    validateString<TIn2>(p.parameters[1]);
  }

  template <class TOut>
  inline uint8_t invokeParamCount(TOut (*invokee)())
  {
    return 0;
  }

  template <class TIn1, class TOut>
  inline uint8_t invokeParamCount(TOut (*invokee)(TIn1))
  {
    return 1;
  }

  template <class TIn1, class TIn2, class TOut>
  inline uint8_t invokeParamCount(TOut (*invokee)(TIn1, TIn2))
  {
    return 2;
  }

  template <class TIn1, class TIn2, class TOut>
  inline PGM_P invokeParamType(TOut (*invokee)(TIn1, TIn2), uint8_t which)
  {
    switch(which)
    {
      case 0:
        return getTypeName<TIn1>();

      case 1:
        return getTypeName<TIn2>();

      default:
        return NULL;
    }
  }
}
