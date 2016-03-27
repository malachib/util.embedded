#pragma once

#include <Arduino.h>
#include "power_base.h"
#include <avr/power.h>

#if defined(__AVR_ATmega32U4__)
// https://harizanov.com/2013/02/power-saving-techniques-on-the-atmega32u4/
#define AVR_USB
#define AVR_TIMER_COUNT 4
#define AVR_USART_COUNT 2
#endif

#if defined(__AVR_ATmega328P__)
#define AVR_PICOPOWER
#endif

// define SWITCH_FN_HELPER_MAX first
#define SWITCH_FN_HELPER(index, prefix, suffix) \
  case index: \
  prefix##index##suffix(); \
  break;

#define SWITCH_FN_POWERON(index, item) SWITCH_FN_HELPER(index, power_##item, _enable)
#define SWITCH_FN_POWEROFF(index, item) SWITCH_FN_HELPER(index, power_##item, _disable)

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

    struct TimerControl
    {
      struct TimerItem
      {
        const uint8_t timer;

        TimerItem(uint8_t timer) : timer(timer) {}

        inline void on()
        {
          switch(timer)
          {
#if AVR_TIMER_COUNT > 0
            case 0:
              power_timer0_enable();
              break;
#endif
#if AVR_TIMER_COUNT > 1
            case 1:
              power_timer1_enable();
              break;
#endif
#if AVR_TIMER_COUNT > 2
            case 2:
              power_timer2_enable();
              break;
#endif
#if AVR_TIMER_COUNT > 3
            case 3:
              power_timer3_enable();
              break;
#endif
#if AVR_TIMER_COUNT > 4
            case 4:
              power_timer4_enable();
              break;
#endif
            default:
              // TODO: find a way to force a compiler error here
              break;
          }
        }

        inline void off()
        {
          switch(timer)
          {
#if AVR_TIMER_COUNT > 0
            case 0:
              power_timer0_disable();
              break;
#endif
#if AVR_TIMER_COUNT > 1
            case 1:
              power_timer1_disable();
              break;
#endif
#if AVR_TIMER_COUNT > 2
            case 2:
              power_timer2_disable();
              break;
#endif
#if AVR_TIMER_COUNT > 3
            case 3:
              power_timer3_disable();
              break;
#endif
            default:
              // TODO: find a way to force a compiler error here
              break;
          }
        }
      };

      TimerItem operator[](uint8_t timer)
      {
        return TimerItem(timer);
      }
    } timer;

    struct UsartControl
    {
      struct UsartItem
      {
        uint8_t index;

        UsartItem(uint8_t index) : index(index) {}

        void on()
        {
          switch(index)
          {
#if AVR_USART_COUNT > 0
            SWITCH_FN_POWERON(0, usart);
#endif
#if AVR_USART_COUNT > 1
            SWITCH_FN_POWERON(1, usart);
#endif
#if AVR_USART_COUNT > 2
            SWITCH_FN_POWERON(2, usart);
#endif
            default:
              break;
          }
        }


        void off()
        {
          switch(index)
          {
#if AVR_USART_COUNT > 0
            SWITCH_FN_POWEROFF(0, usart);
#endif
#if AVR_USART_COUNT > 1
            SWITCH_FN_POWEROFF(1, usart);
#endif
#if AVR_USART_COUNT > 2
            SWITCH_FN_POWEROFF(2, usart);
#endif
            default:
              break;
          }
        }
      };

      UsartItem operator[](uint8_t index)
      {
        return UsartItem(index);
      }

    } usart;

    struct TwiControl
    {
      static inline void on()
      {
        power_twi_enable();
      }

      static inline void off()
      {
        power_twi_disable();
      }

    } twi;


    struct SpiControl
    {
      static inline void on()
      {
        power_spi_enable();
      }

      static inline void off()
      {
        power_spi_disable();
      }

    } spi;
  };

  extern PowerControl Power;
}
