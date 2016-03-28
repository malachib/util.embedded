#pragma once

// sleep modes http://asf.atmel.com/docs/3.16.0/samd21/html/group__asfdoc__sam0__system__group.html#asfdoc_sam0_system_module_overview_sleep_mode

namespace FactUtilEmbedded
{
  class PowerControl
  {
  public:
    // puts system to sleep waiting for interrupt
    static void sleep(const system_sleepmode sleep_mode = SYSTEM_SLEEPMODE_STANDBY)
    {
      system_set_sleepmode(sleep_mode);
      system_sleep();
    }
  };

  extern PowerControl Power;
}
