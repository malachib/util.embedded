//#include <Arduino.h>
#include "driver/gnss.h"

namespace driver
{
  const __FlashStringHelper* IGNSS_raw::getFieldString(GnssFields field)
  {
    switch(field)
    {
      case GNSS_STATUS_RUN: return F("status run");
      case GNSS_LATITUDE: return F("latitude");
      case GNSS_LONGITUDE: return F("longitude");
      case GNSS_STATUS_FIX: return F("status fix");
      case GNSS_ALTITUDE: return F("altitude");
      case GNSS_TIMESTAMP: return F("timestamp");
      case GNSS_SPEED: return F("speed");

      default: return F("unknown");
    }
  }
}
