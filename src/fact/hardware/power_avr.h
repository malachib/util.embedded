#pragma once

#include <Arduino.h>
#include "power_base.h"

#if defined(__AVR_ATmega32U4__)
#define AVR_USB
#endif

namespace FactUtilEmbedded
{
  class PowerControl : public PowerControlBase
  {
  public:
#ifdef AVR_USB
    static inline void usbOff()
    {
      USBCON |= _BV(FRZCLK);  //freeze USB clock
      PLLCSR &= ~_BV(PLLE);   // turn off USB PLL
      USBCON &= ~_BV(USBE);   // disable USB
    }

    static inline void usbOn()
    {
      delay(100);             // TBD: see if this delay is really necessary
      USBDevice.attach();
      delay(100);             // this delay sources emphatically say is necessary
    }
#endif
  };
}
