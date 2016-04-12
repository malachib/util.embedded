#pragma once

#include <Arduino.h>
#include "power_avr.h"
#include "watchdog_avr.h"
#include "avr/features.h"
#include <avr/sleep.h>

// http://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html
namespace FactUtilEmbedded
{
  // Sleep offers integration between Watchdog and Power modules,
  // as well as convenience calls for calling into Power modes
  // thus the seeming redundancy.
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
    // to turn things back on.  Once sleep is over, watchdog is
    // disabled
    void powerDown(const uint8_t wdto PARAM_BOD_DISABLE)
    {
      Watchdog::isr.on();
      Watchdog::enable(wdto);
      powerDown(PARAM_BOD_CALL_S);
      Watchdog::disable();
    }
  };
  
  extern SleepControl Sleep;
}
