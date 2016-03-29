#pragma once

#include <Arduino.h>
#include "power_avr.h"
#include "watchdog_avr.h"
#include "avr/features.h"
#include <avr/sleep.h>

// http://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html
namespace FactUtilEmbedded
{
  class SleepControl
  {
  public:
    void idle()
    {
      Power.sleep(SLEEP_MODE_IDLE);
    }
    
    void powerDown(PARAM_BOD_DISABLE_S)
    {
      Power.sleep(SLEEP_MODE_PWR_DOWN PARAM_BOD_CALL);
    }

    // enters AVR power down state with assistance of Watchdog
    // to turn things back on
    void powerDown(uint8_t interval PARAM_BOD_DISABLE)
    {
      Watchdog.isr.on();
      Watchdog.enable(interval);
      powerDown(PARAM_BOD_CALL_S);
    }
  };
  
  extern SleepControl Sleep;
}
