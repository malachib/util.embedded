#ifdef __AVR__
#include "watchdog_avr.h"

// special thanks all over the place.  Code lifted from everywhere:
// https://github.com/janelia-arduino/Watchdog/blob/master/Watchdog.cpp
// https://github.com/combs/Arduino-Watchdog-Handler/blob/master/watchdog.h
// https://github.com/mchr3k/arduino/blob/master/libraries/ATTinyWatchdog/ATTinyWatchdog.cpp
// https://github.com/adafruit/Adafruit_SleepyDog
// Unfortunately, none of them were all of these conditions:
// 1) well organized
// 2) supporting of both attiny and atmega series
//
// the ATTinyWatchdog link is especially informative

#if defined(__AVR_ATtiny85__)
#define ATTINY
#endif

#ifndef WATCHDOG_FEATURE_ISR_SUPPRESS
ISR(WDT_vect) 
{
  // Don't do anything here but we must include this
  // block of code otherwise the interrupt calls an
  // uninitialized interrupt handler.
}
#endif

namespace FactUtilEmbedded
{
  WatchdogControl Watchdog;
  
  uint8_t WatchdogControl::buildPrescalar(const uint8_t wdto) const
  {
  #ifdef ATTINY
      // 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
      // 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
      uint8_t bb;

      //if (wdto > 9 ) wdto=9;
      bb=wdto & 7;
      if (wdto > 7) bb|= (1<<5);
      bb|= (1<<WDCE);
      return bb;
  #else
      // Build watchdog prescaler register value before timing critical code.
      uint8_t wdps = ((wdto & 0x08 ? 1 : 0) << WDP3) |
                     ((wdto & 0x04 ? 1 : 0) << WDP2) |
                     ((wdto & 0x02 ? 1 : 0) << WDP1) |
                     ((wdto & 0x01 ? 1 : 0) << WDP0);
      return wdps;
  #endif  
  }
  
  #ifdef ATTINY
  #define _WDTCR WDTCR
  #else
  #define _WDTCR WDTCSR
  #endif
  
  void WatchdogControl::enable(const uint8_t wdto) const
  {
    uint8_t wd_control = buildPrescalar(wdto);
    
    // enable system reset function, if desired
    if(systemReset()) wd_control |= (1 << WDE);
    
    // enable ISR call function, if desired
    if(isr()) wd_control |= (1 << WDIE);
    
    // The next section is timing critical so interrupts are disabled.
    cli();
    // First clear any previous watchdog reset.
    MCUSR &= ~(1<<WDRF);
    // Now change the watchdog prescaler and interrupt enable bit so the watchdog
    // reset only triggers the interrupt (and wakes from deep sleep) and not a
    // full device reset.  This is a timing critical section of code that must
    // happen in 4 cycles.
    _WDTCR |= (1<<WDCE) | (1<<WDE);  // Set WDCE and WDE to enable changes.
    _WDTCR = wd_control;             // Set the prescaler bit values + feature enable
    // Critical section finished, re-enable interrupts.
    sei();
  }
}


#endif
