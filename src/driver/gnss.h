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
  VIRTUAL void initializeContext(void* ctx) { };

  //VIRTUAL bool read() ABSTRACT;
  VIRTUAL void getGPS(GNSS_raw* data) ABSTRACT;
};


class IGNSS : public IDriver
{
public:
  VIRTUAL bool getGPS(double* latitude, double* longitude,
    double* speed_kph = NULL,
    double* heading = NULL,
    double* altitude = NULL) ABSTRACT;
};
