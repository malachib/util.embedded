#pragma once

#include <sam.h>

// good overview of SAMD power management: http://www.atmel.com/Images/Atmel-42248-SAM-D20-Power-Measurements_ApplicationNote_AT04188.pdf

// sleep modes http://asf.atmel.com/docs/3.16.0/samd21/html/group__asfdoc__sam0__system__group.html#asfdoc_sam0_system_module_overview_sleep_mode
//#include <asf.h>
// getting ASF is kind of a pain, so going direct to registers like these folks have done:
//   https://github.com/rocketscream/Low-Power/blob/master/LowPower.cpp
//   https://github.com/arduino-libraries/RTCZero/blob/master/src/RTCZero.cpp
// Note their deviation from the official ASF code:
//   https://github.com/avrxml/asf/blob/4a77ebe534c2f0386730ad8d3a03168f22c23e22/sam0/drivers/system/power/power_sam_l/power.h
// Note also Adafruit made a copy of ASF for themselves https://github.com/adafruit/Adafruit_ASFcore
// official code never touches SCB reg, and the non-official code doesn't touch PM->SLEEP.reg as often

#define GENERIC_CONTROL_STRUCT(name, instance, target) \
struct name \
{ \
  static void on() { target = 1; } \
  static void off() { target = 0; } \
} instance;

// Since SAM0 series has a full blown Power Manager, this class maps largely to
// that
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
    
    struct ClockControl
    {
      GENERIC_CONTROL_STRUCT(Usb2, usb, PM->APBBMASK.bit.USB_);
      
      struct ApbControl
      {
        class ApbItem
        {
          const uint8_t bus;
          
        public:
          ApbItem(const uint8_t bus) : bus(bus) {}
          
          void setDivider(uint8_t p)
          {
            switch(bus)
            {
              case 0: PM->APBASEL.reg = p; break;
              case 1: PM->APBBSEL.reg = p; break;
              case 2: PM->APBCSEL.reg = p; break;
            }
          }

          template <uint8_t p>
          void setDivider()
          {
            static_assert(p < 8, "prescalar must be less than 8");
            setDivider(p);
          }
          
        };

        const ApbItem operator[](uint8_t bus) const
        {
          return ApbItem(bus);
        }
      } apb;
      
    };

    static ClockControl clock;
    
    // stubs while I understand SAMD better and verify other things are compiling
    struct UsbControl 
    {
      // TODO: Pretty sure we have to do the startup./shutdown tricks
      // similar to the AVR
      static void on() { clock.usb.on(); }
      static void off() { clock.usb.off(); }
    } usb;

    struct AdcControl 
    {
      void on() {}
      void off() {}
    } adc;

    static inline void sleep()
    {
      // lifted straight from ASF.  Note: I think I'd want this as a default
      // system wide anyway, so consider moving this elsewhere
#if SAMD_SERIES
      /* Errata: Make sure that the Flash does not power all the way down
       * when in sleep mode. */
      NVMCTRL->CTRLB.bit.SLEEPPRM = NVMCTRL_CTRLB_SLEEPPRM_DISABLED_Val;
#endif
      
      // Data Synchronization Barrier
      __DSB();
      // wait for interrupt to wake us up
      __WFI();
    }
    
    // from datasheet:
    // IDLE mode: The CPU is stopped. Optionally, some synchronous clock domains are stopped,
    // depending on the IDLE argument. Regulator operates in normal mode.
    // 
    // valid levels are 0-2:
    //
    // 0: CPU clock stopped
    // 1: CPU and AHB stopped
    // 2: CPU, AHB and APB domains stopped
    //
    // code not complete
    static void idle(uint8_t level)
    {
      SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
      PM->SLEEP.reg = level;
      sleep();
    }
    
    //
    // In STANDDBY sleep mode, the power manager is frozen and is able to go back to ACTIVE mode upon
    // any asynchronous interrupt
    static void standby()
    {
      // enable deepsleep AKA standby mode
      SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
      sleep();
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
