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
    // adaptation of ASF libs in a more C++ (and likely more optimized)
    // form
    class APB
    {
      // apb_bus 0 = A, 1 = B, 2 = C
      template <uint8_t apb_bus>
      static inline volatile uint32_t& get_clock_mask()
      {
        static_assert(apb_bus >= 0 && apb_bus <= 2,
          "APB bus must be 0, 1 or 2 (corresponding to A, B and C)");

        switch(apb_bus)
        {
          case 0: return PM->APBAMASK.reg;
          case 1: return PM->APBBMASK.reg;
          case 2: return PM->APBBMASK.reg;
        }
      }

    public:
      // apb_bus 0 = A, 1 = B, 2 = C
      template <uint8_t apb_bus>
      static inline void clock_set_mask(const uint32_t mask)
      {
        get_clock_mask<apb_bus>() |= mask;
      }

      template <uint8_t apb_bus>
      static inline void clock_clear_mask(const uint32_t mask)
      {
        get_clock_mask<apb_bus>() &= ~mask;
      }
    };
  }
  }
}
