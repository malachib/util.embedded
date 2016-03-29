#pragma once

// lots of code adapted/lifted from www.rocketscream.com / https://github.com/rocketscream/Low-Power
// good ref one sleep modes here: http://www.engblaze.com/hush-little-microprocessor-avr-and-arduino-sleep-mode-basics/

#include <Arduino.h>
#include "power_base.h"
#include <avr/power.h>
#include <avr/sleep.h>
#include "avr/features.h"

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
        USBCON |= _BV(FRZCLK);  //freeze USB clock
        PLLCSR &= ~_BV(PLLE);   // turn off USB PLL
        USBCON &= ~_BV(USBE);   // disable USB

        // have to disable after, otherwise it seems not all the above commands run
        // and USB stays pseudo-connected to host
        power_usb_disable();
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
        ADCSRA &= ~(1 << ADEN);
        power_adc_disable();
      }


      static inline void on()
      {
        power_adc_enable();
        ADCSRA |= (1 << ADEN);
      }
    } adc;

    struct TimerControl
    {
      struct TimerItem
      {
        const uint8_t timer;

        TimerItem(uint8_t timer) : timer(timer) {}

        inline void on() const
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

        inline void off() const
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

      const TimerItem operator[](uint8_t timer) const
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

        void on() const
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


        void off() const
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

      const UsartItem operator[](uint8_t index) const
      {
        return UsartItem(index);
      }

    } usart;

#ifdef AVR_TWI
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
#endif

#ifdef AVR_SPI
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
#endif

#ifdef AVR_USI
    struct UsiControl
    {
      static inline void on()
      {
        power_usi_enable();
      }

      static inline void off()
      {
        power_usi_disable();
      }
    } usi;
#endif

    // experimental, not ready
    struct Command
    {
#ifdef AVR_SPI
      bool spi;
#endif
#ifdef AVR_TWI
      bool twi;
#endif
#ifdef AVR_PICOPOWER
      bool bod;
#endif
#ifdef AVR_USB
      bool usb;
#endif

      void sleep()
      {
        // TBD:
      }
    };

    // be sure to choose the specific mode for the MPU at hand, as they
    // all vary slightly.  For example 32u4 & 328P have SLEEP_MODE_STANDBY, but
    // attiny85 does not
    static void sleep(uint8_t mode = SLEEP_MODE_PWR_SAVE
#ifdef AVR_PICOPOWER
      // it is assumed bod is on by default, and also assumed bod should be
      // switched back on after sleep
      ,bool bod_disable = false
#endif
    )
    {
      set_sleep_mode(mode);
      cli();
      sleep_enable();
      // beware: this is the preferred line to set up recurring ISRs, according
      // to http://playground.arduino.cc/Learning/ArduinoSleepCode
      // however, it seems to me merely setting cli() earlier should do the trick
#ifdef AVR_PICOPOWER
      if(bod_disable) { sleep_bod_disable(); }
#endif
      sei();
      sleep_cpu();
      // will wake up here
      sleep_disable();
#ifdef AVR_PICOPOWER
/*
  // may have to explicitly define this ourselves
  MCUCR &= ~_BV(BODS);
 */
      if(bod_disable) { sleep_bod_enable(); }
#endif
      sei(); // TODO: figure out if we need that 2nd sei here.  RocketStream's lib does, but others dont
    }
  };

  extern PowerControl Power;
}
