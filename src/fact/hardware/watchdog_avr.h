#pragma once

#include <Arduino.h>
#include <avr/wdt.h>

namespace FactUtilEmbedded
{
  /*
  namespace Watchdog
  {
    
  }; */
  
  class Watchdog
  {
    static uint8_t buildPrescalar(const uint8_t wdto);
    static void enableFromPrescalar(uint8_t wd_control);

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
        static_assert(timeout == 8000
          || timeout == 4000
          || timeout == 2000
          || timeout == 1000
          || timeout == 500
          || timeout == 250
          || timeout == 120
          || timeout == 60
          || timeout == 30
          || timeout == 15,
          "Invalid WDTO timeout value requested"
        );

        switch(timeout)
        {
          case 8000: return WDTO_8S;
          case 4000: return WDTO_4S;
          case 2000: return WDTO_2S;
          case 1000: return WDTO_1S;
          case 500: return WDTO_500MS;
          case 250: return WDTO_250MS;
          case 120: return WDTO_120MS;
          case 60: return WDTO_60MS;
          case 30: return WDTO_30MS;
          case 15: return WDTO_15MS;
          default:
            return -1; // should never get here
        }
      }    
    };
    
    struct Control
    {
      void on()
      {
        enabled = true;
      }

      void off()
      {
        enabled = false;
      }

      bool isEnabled() const { return enabled; }
      bool operator()() const { return enabled; }

    private:
      bool enabled = false;
    };

    static Control isr;
    static Control systemReset;

    // TODO: Turn this into a non-static class because it's cleaner -
    // but first we need to verify that a non-static class won't cause
    // code bloat
    class Prescalar
    {
      static uint8_t cachedPrescalar;

    public:
      static uint8_t build(const uint8_t wdto) { return buildPrescalar(wdto); }
      void cache(const uint8_t wdto) { cachedPrescalar = build(wdto); }

      static uint8_t getCached() { return cachedPrescalar; }

    };

    static Prescalar prescalar;

    template <uint16_t timeout>
    static WDTO getWDTOfromMS() { return WDTO::fromMS<timeout>(); }

    // "Reset the watchdog timer. When the watchdog timer is enabled,
    // a call to this instruction is required before the timer expires,
    // otherwise a watchdog-initiated device reset will occur."
    // - http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__watchdog.html
    static void reset() { wdt_reset(); }
    static void disable() { wdt_disable(); }

    static void enable(const uint8_t wdto);

// takes 2 extra code bytes on attiny85, even when dormant...
#ifndef WATCHDOG_FEATURE_CACHE_SUPPRESS
    // enable from cached prescalar
    static void enable();
#endif
  };
}
