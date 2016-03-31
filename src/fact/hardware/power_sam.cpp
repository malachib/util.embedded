#include <Arduino.h>

#if SAM_SERIES
#include "power_sam.h"

namespace FactUtilEmbedded
{
  PowerControl Power;
  PowerControl::ClockControl PowerControl::clock;
}
#endif
