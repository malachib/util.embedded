#pragma once

#include <Arduino.h>
#include <avr/wdt.h>

namespace FactUtilEmbedded
{
  class WatchdogControl
  {
    static uint8_t buildPrescalar(const uint8_t wdto);

  public:
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

    // "Reset the watchdog timer. When the watchdog timer is enabled,
    // a call to this instruction is required before the timer expires,
    // otherwise a watchdog-initiated device reset will occur."
    // - http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__watchdog.html
    static void reset() { wdt_reset(); }
    static void disable() { wdt_disable(); }

    // did I just rewrite this?
    // http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__watchdog_1gaf6cfea2a1b61e2530ea0c4ef8fc572b3.html
    static void enable(const uint8_t wdto);
  };

  extern WatchdogControl Watchdog;
}
