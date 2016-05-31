#pragma once

//#include <wdt.h>

#include "../../driver/atmel/sam/gclk.h"

// http://asf.atmel.com/docs/latest/samd20/html/group__asfdoc__sam0__wdt__group.html
// code totally lifted & adapted from https://github.com/adafruit/Adafruit_SleepyDog

namespace FactUtilEmbedded
{
  using namespace atmel::sam;

  class Watchdog
  {
  public:
#ifdef ASF_ENABLED
    static void reset() { wdt_reset_count(); }
    static void enable(wdt_period timeout_period)
    {
      struct wdt_conf config;

      wdt_get_config_defaults(&config);

      // defaults to Generic Clock Channel 4
      config.enable = true;
      config.timeout_period = timeout_period;

      wdt_set_config(&config);
    }

    // works only for early warning, SAMD seems to only offer that
    static status_code register_callback(const wdt_callback_t callback)
    {
      return wdt_register_callback(callback, WDT_CALLBACK_EARLY_WARNING);
    }
#else
  private:
    // TODO: ensure busy wait loop doesn't go for too long, if so, use
    // delay()
    inline static void synchronize()
    {
      // Syncronize write to reg written to before calling this
      while (WDT->STATUS.bit.SYNCBUSY);
    }

  public:
    class WDTO
    {
      // only allow WDTO to construct these from the
      // mini-factory fromMS
      //friend WatchdogControl;

      const uint8_t value;

      WDTO(const uint8_t value) : value(value) {}
    public:

      operator uint8_t() const { return value; }

      template <uint16_t timeout>
      static WDTO fromMS()
      {
        // TODO: powers of two we should be able to fold down this
        // logic
        static_assert(timeout == 16384
          || timeout == 8192
          || timeout == 4096
          || timeout == 2048
          || timeout == 1024
          || timeout == 512
          || timeout == 256
          || timeout == 128
          || timeout == 64
          || timeout == 32
          || timeout == 16
          || timeout == 8,
          "Invalid WDTO timeout value requested"
        );

        switch(timeout)
        {
          case 16384: return 11;
          case 8192:  return 10;
          case 4096:  return 9;
          case 2048:  return 8;
          case 1024:  return 7;
          case 512:   return 6;
          case 256:   return 5;
          case 128:   return 4;
          case 64:    return 3;
          case 32:    return 2;
          case 16:    return 1;
          case 8:     return 0;
          default:
            return -1; // should never get here
        }
      }
    };


    // lifted from adafruit
    // TODO: figure out if it's safe to do this at static-init time
    static void initialize()
    {
      // Do one-time initialization of the watchdog timer.

      // Setup GCLK for the watchdog using:
      // - Generic clock generator 2 as the source for the watchdog clock
      // - Low power 32khz internal oscillator as the source for generic clock
      //   generator 2.
      // - Generic clock generator 2 divisor to 32 so it ticks roughly once a
      //   millisecond.

      // Set generic clock generator 2 divisor to 4 so the clock divisor is 32.
      // From the datasheet the clock divisor is calculated as:
      //   2^(divisor register value + 1)
      // A 32khz clock with a divisor of 32 will then generate a 1ms clock period.
      GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(4);
      // Now enable clock generator 2 using the low power 32khz oscillator and the
      // clock divisor set above.
      GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) |
                          GCLK_GENCTRL_GENEN |
                          GCLK_GENCTRL_SRC_OSCULP32K |
                          GCLK_GENCTRL_DIVSEL;
      while (GCLK->STATUS.bit.SYNCBUSY);  // Syncronize write to GENCTRL reg.
      // Turn on the WDT clock using clock generator 2 as the source.
      GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_WDT |
                          GCLK_CLKCTRL_CLKEN |
                          GCLK_CLKCTRL_GEN_GCLK2;

    }


    // untested, will replace initialize() once it is tested and working
    template <uint8_t gclk = 2>
    static void initialize2()
    {
      GenericClock::set_divisor<gclk>(4);

      // Now enable clock generator "gclk" using the low power 32khz oscillator and the
      // clock divisor set above.
      GenericClock::enable_generator<gclk>(
        GCLK_GENCTRL_SRC_OSCULP32K |
        GCLK_GENCTRL_DIVSEL);

      GenericClock::enable_clock<gclk>(GCLK_CLKCTRL_ID_WDT);
    }


    inline static bool getEarlyWarningStatus()
    {
      return WDT->INTFLAG.bit.EW;
    }

    inline static void disable()
    {
      // Disable the watchdog.
      WDT->CTRL.reg &= ~WDT_CTRL_ENABLE;
      synchronize();
    }


    // raw watchdog enable
    inline static void enable()
    {
      // Enable the watchdog.
      WDT->CTRL.reg |= WDT_CTRL_ENABLE;
      synchronize();
    }

    inline static void enableInterrupt()
    {
      // Just aping what I see in RTCZero library
      NVIC_EnableIRQ(WDT_IRQn);
      NVIC_SetPriority(WDT_IRQn, 0x00);
    }

    inline static void disableEarlyWarningInterrupt()
    {
      // Disable early warning interrupt.
      WDT->INTENCLR.reg |= WDT_INTENCLR_EW;
    }

    inline static void enableEarlyWarningInterrupt()
    {
      // Disable early warning interrupt.
      WDT->INTENSET.reg |= WDT_INTENSET_EW;
    }

    inline static void disableInterrupt()
    {
      NVIC_DisableIRQ(WDT_IRQn);
    }

    static void enable(const WDTO wdto)
    {
      // Disable watchdog so its registers can be changed
      disable();

      // Set watchdog period
      WDT->CONFIG.reg = WDT_CONFIG_PER(wdto);
      synchronize();

      // Disable early warning interrupt.
      disableEarlyWarningInterrupt();

      enable();
    }

    static void reset()
    {
      // Write the watchdog clear key value (0xA5) to the watchdog clear register
      // to clear the watchdog timer and reset it.
      WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
      synchronize();
    }
#endif
  };
}
