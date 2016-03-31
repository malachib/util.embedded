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
    
    // stubs while I understand SAMD better and verify other things are compiling
    struct UsbControl 
    {
      void on() {}
      void off() {}
    } usb;
    
    struct AdcControl 
    {
      void on() {}
      void off() {}
    } adc;
    
  
    // from datasheet:
    // IDLE mode: The CPU is stopped. Optionally, some synchronous clock domains are stopped,
    // depending on the IDLE argument. Regulator operates in normal mode.
    // 
    // valid levels are 0-2
    //
    // code not complete
    void idle(uint8_t level)
    {
      SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
      PM->SLEEP.reg = level;
      
      __DSB();
      // wait for interrupt to wake us up
      __WFI();
    }
    
    //
    // In STANDDBY sleep mode, the power manager is frozen and is able to go back to ACTIVE mode upon
    // any asynchronous interrupt
    void standby()
    {
      SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
      __DSB();
      __WFI();
    }

    // non-cpu clocks can temporarily wakeup to then trigger (or not) a wakeup
    // of the CPU itself
    void sleepWalk()
    {
      
    }

    template <uint8_t p>
    inline void cpuPrescale()
    {
      static_assert(p < 8, "prescalar must be less than 8");

      PM->CPUSEL.reg = p;
    }
  };

  extern PowerControl Power;
}
