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
    class TimerCounter
    {
      template <uint8_t tc_index>
      static void assert_tc_index()
      {
        //assert_static()
      }

    public:
      static Tc* get_instance()
      {
        return nullptr;
      }

      /*
      static uint8_t get_instance_index()
      {

      }
      */

      static void config()
      {

      }
    };
  }
  }
}
