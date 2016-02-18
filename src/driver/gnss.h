#pragma once

#include "core.h"

struct GNSS_raw
{
  char* latitude;
  char* longitude;
};

struct GNSS_raw_ext : GNSS_raw
{
  char* timestamp;
  char* speed;
  char* heading;
  char* altitude;
};

class IGNSS_raw : public IDriver
{
public:
  VIRTUAL uint16_t getContextSize() ABSTRACT;
  VIRTUAL void initContext(void* ctx) { };

  //VIRTUAL bool read() ABSTRACT;
  VIRTUAL bool getGNSS(void* context, GNSS_raw_ext* data) ABSTRACT;
};


class IGNSS : public IDriver
{
public:
  VIRTUAL bool getGNSS(double* latitude, double* longitude,
    double* speed_kph = NULL,
    double* heading = NULL,
    double* altitude = NULL) ABSTRACT;
};
