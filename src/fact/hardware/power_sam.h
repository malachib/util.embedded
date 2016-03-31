#pragma once

// sleep modes http://asf.atmel.com/docs/3.16.0/samd21/html/group__asfdoc__sam0__system__group.html#asfdoc_sam0_system_module_overview_sleep_mode
//#include <asf.h>
// getting ASF is kind of a pain, so going direct to registers like these folks have done:
//   https://github.com/rocketscream/Low-Power/blob/master/LowPower.cpp
//   https://github.com/arduino-libraries/RTCZero/blob/master/src/RTCZero.cpp
// Note their deviation from the official ASF code:
//   https://github.com/avrxml/asf/blob/4a77ebe534c2f0386730ad8d3a03168f22c23e22/sam0/drivers/system/power/power_sam_l/power.h
// official code never touches SCB reg, and the non-official code doesn't touch PM->SLEEP.reg as often

namespace FactUtilEmbedded
{
  class PowerControl
  {
  public:
    /* needs ASF
    // puts system to sleep waiting for interrupt
    static void sleep(const system_sleepmode sleep_mode = SYSTEM_SLEEPMODE_STANDBY)
    {
      system_set_sleepmode(sleep_mode);
      system_sleep();
    }*/
    
    void standby()
    {
      SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
      __DSB();
      __WFI();
    }
  };

  extern PowerControl Power;
}
