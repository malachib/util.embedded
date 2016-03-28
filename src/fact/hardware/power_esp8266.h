#pragma once

#include "power_base.h"
#include <Arduino.h>
//#include "gpio.h"

extern "C"
{
  #include "user_interface.h"

  sleep_type_t wifi_get_sleep_type(void);
}

// good intel: https://www.sparkfun.com/news/1842
// http://russ.russmathis.com/wp-content/uploads/2015/09/9B-ESP8266__Sleep__Function-Description__EN_v1.0.pdf
// http://russ.russmathis.com/esp8266-power-modes/
// https://github.com/esp8266/Arduino/issues/1381

namespace FactUtilEmbedded
{
  class PowerControl : public PowerControlBase
  {
  public:
    // FIX: determine if this should actually be Restart, maybe RAM will be
    // intact?
    static void sleepAndReset(uint32_t timeout, WakeMode mode = RF_DEFAULT)
    {
      ESP.deepSleep(timeout, mode);
    }

    // maps to LIGHT SLEEP mode.  Documents say this says to enable
    // sleep mode automatically when low activity in STA mode only.
    // furthermore some docs indicate this mode is the default for the part
    static void lightSleep()
    {
      wifi_set_sleep_type(LIGHT_SLEEP_T);
    }
  };

  extern PowerControl Power;
}
