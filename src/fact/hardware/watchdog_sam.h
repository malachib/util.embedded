#pragma once

//#include <wdt.h>

// http://asf.atmel.com/docs/latest/samd20/html/group__asfdoc__sam0__wdt__group.html
namespace FactUtilEmbedded
{
  class WatchdogControl
  {
  public:
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
  };
}
