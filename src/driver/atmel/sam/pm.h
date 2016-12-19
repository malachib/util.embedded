#pragma once

#include <stdint.h>
#include <sam.h>

#if SAMD_SERIES == 0
#error This code intended only for ATSAMD MPU
#endif

namespace FactUtilEmbedded
{
  namespace atmel
  {
  namespace sam
  {
    class PowerManager
    {
      /*
      template <uint32_t& TOffset, uint32_t mask>
      class _EnableDisableMask
      {
      public:
        static void inline enable()
        {
          TOffset.reg |= mask;
        }

        static void inline disable()
        {
          TOffset.reg &= ~mask;
        }
      };


      class __RTC :
      public _EnableDisableMask<PM->APBAMASK, PM_APBAMASK_RTC> {};*/

      /*
      class RTC
      {
      public:
        static inline enable()
        {
          PM->APBAMASK.reg |= PM_APBAMASK_RTC; // turn on digital interface clock
        }
      };*/
    };
  }
  }
}
