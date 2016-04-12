#pragma once

#include <Arduino.h>
#include <avr/wdt.h>

namespace FactUtilEmbedded
{
  class WatchdogControl
  {
    static uint8_t buildPrescalar(const uint8_t wdto);
    static void enableFromPrescalar(uint8_t wd_control);
    
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
    
    class Prescalar
    {
      static uint8_t cachedPrescalar;
    
    public:
      static uint8_t build(const uint8_t wdto) { return buildPrescalar(wdto); }
      void cache(const uint8_t wdto) { cachedPrescalar = build(wdto); }
      
      static uint8_t getCached() { return cachedPrescalar; }
      
    } prescalar;
    
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
  
  extern WatchdogControl Watchdog;
}
