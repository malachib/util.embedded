#pragma once

#include <Arduino.h>

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
      
    }
  }
  }
}
