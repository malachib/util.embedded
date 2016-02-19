#pragma once

#include "core.h"

namespace driver {

#define DEVICE_GNSS_FEATURE_DECIMAL 0x01

enum GnssFields : uint8_t
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


class IGNSS_raw : public IDriver
{
public:
  typedef bool (*token_callback)(GnssFields field, char* value, void* context);

  VIRTUAL uint16_t getContextSize() ABSTRACT;
  VIRTUAL void initContext(void* ctx) { };

  //VIRTUAL bool read() ABSTRACT;
  VIRTUAL bool getGNSS(token_callback callback, void* context = NULL) ABSTRACT;

  static const __FlashStringHelper* getFieldString(GnssFields field);
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
