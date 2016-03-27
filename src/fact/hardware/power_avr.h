#pragma once

#include <Arduino.h>
#include "power_base.h"
#include <avr/power.h>

#if defined(__AVR_ATmega32U4__)
// https://harizanov.com/2013/02/power-saving-techniques-on-the-atmega32u4/
#define AVR_USB
#endif

#if defined(__AVR_ATmega328P__)
#define AVR_PICOPOWER
#endif

namespace FactUtilEmbedded
{
  class PowerControl : public PowerControlBase
  {
  public:
#ifdef AVR_USB
    struct USBControl
    {
      static inline void off()
      {
        power_usb_disable();

        USBCON |= _BV(FRZCLK);  //freeze USB clock
        PLLCSR &= ~_BV(PLLE);   // turn off USB PLL
        USBCON &= ~_BV(USBE);   // disable USB
      }

      static inline void on()
      {
        power_usb_enable();

        delay(100);             // TBD: see if this delay is really necessary
        USBDevice.attach();
        delay(100);             // this delay sources emphatically say is necessary
      }
    } usb;
#endif

    struct ADCControl
    {
      static inline void off()
      {
        power_adc_disable();
      }


      static inline void on()
      {
        power_adc_enable();
      }
    } adc;
  };

  extern PowerControl Power;
}
