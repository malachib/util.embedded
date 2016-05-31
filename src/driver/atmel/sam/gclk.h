#pragma once

#include <Arduino.h>

#if SAMD_SERIES == 0
#error This code intended only for ATSAMD MPU
#endif

#include "apb.h"

namespace FactUtilEmbedded
{
  namespace atmel
  {
  namespace sam
  {
    // adaptation of ASF libs in a more C++ (and likely more optimized)
    // form
    class GenericClock
    {
      static inline bool is_syncing()
      {
        return GCLK->STATUS.bit.SYNCBUSY;
        //return GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY;
      }

      static void synchronize()
      {
        while(is_syncing());
      }

      static void reset()
      {
        GCLK->CTRL.reg = GCLK_CTRL_SWRST;
        // Wait for reset to complete
        while(GCLK->CTRL.reg & GCLK_CTRL_SWRST);
      }

      static void initialize()
      {
        APB::clock_set_mask<0>(PM_APBAMASK_GCLK);

        // Software reset the module to ensure it is re-initialized correctly
        reset();
      }

      template <uint8_t gclk>
      static void assert_gclk()
      {
        // MB: Pretty sure we have 8 to choose from
        static_assert(gclk >= 0 && gclk < 8,
          "invalid gclk specified");
      }

      template <uint8_t gclk>
      static const uint32_t get_ctrl_gen_gclk()
      {
        assert_gclk<gclk>();

        switch(gclk)
        {
          case 0: return GCLK_CLKCTRL_GEN_GCLK0;
          case 1: return GCLK_CLKCTRL_GEN_GCLK1;
          case 2: return GCLK_CLKCTRL_GEN_GCLK2;
          case 3: return GCLK_CLKCTRL_GEN_GCLK3;
          case 4: return GCLK_CLKCTRL_GEN_GCLK4;
          case 5: return GCLK_CLKCTRL_GEN_GCLK5;
          case 6: return GCLK_CLKCTRL_GEN_GCLK6;
          case 7: return GCLK_CLKCTRL_GEN_GCLK7;
        }
      }

    public:
      // Set generic clock generator "gclk" divisor to "divisor"
      // (i.e. if root clock speed is 32khz and divisor is 4, the clock divisor is 32)
      // From the datasheet the clock divisor is calculated as:
      //   2^(divisor register value + 1)
      // A 32khz clock with a divisor of 32 will then generate a 1ms clock period.
      template <uint8_t gclk>
      static void set_divisor(uint8_t divisor)
      {
        assert_gclk<gclk>();

        GCLK->GENDIV.reg = GCLK_GENDIV_ID(gclk) | GCLK_GENDIV_DIV(divisor);
      }


      template <uint8_t gclk>
      static inline void enable_generator(const uint32_t GCLK_GENCTRL)
      {
        assert_gclk<gclk>();

        GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(gclk) |
                            GCLK_GENCTRL_GENEN |
                            GCLK_GENCTRL;

        synchronize();
      }

      // enable a particular generic clock coupled with a peripheral
      // identified by GCLK_CTRLCTRL_ID_xxx
      template <uint8_t gclk>
      static inline void enable_clock(const uint32_t GCLK_CLKCTRL_ID)
      {
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID |
                            GCLK_CLKCTRL_CLKEN |
                            get_ctrl_gen_gclk<gclk>();
      }
    };
  }
  }
}
