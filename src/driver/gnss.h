#pragma once

#include "core.h"

namespace driver {

#define DEVICE_GNSS_FEATURE_DECIMAL 0x01

enum GnssFields
{
  GNSS_LATITUDE,
  GNSS_LONGITUDE,
  GNSS_SPEED,
  GNSS_HEADING,
  GNSS_TIMESTAMP,
  GNSS_ALTITUDE,
  GNSS_LATP,
  GNSS_LONGP,
  GNSS_STATUS_RUN,
  GNSS_STATUS_FIX
};


typedef bool (*IGNSS_raw_token_callback)(GnssFields field, char* value, void* context);

class IGNSS_raw : public IDriver
{
public:
  VIRTUAL uint16_t getContextSize() ABSTRACT;
  VIRTUAL void initContext(void* ctx) { };

  //VIRTUAL bool read() ABSTRACT;
  VIRTUAL bool getGNSS(IGNSS_raw_token_callback callback, void* context = NULL) ABSTRACT;
};


class IGNSS : public IDriver
{
public:
  VIRTUAL bool getGNSS(double* latitude, double* longitude,
    double* speed_kph = NULL,
    double* heading = NULL,
    double* altitude = NULL) ABSTRACT;
};
}
