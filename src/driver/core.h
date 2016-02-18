#pragma once

#include "../features.h"

#ifdef DRIVER_FEATURE_VTABLE
#define VIRTUAL virtual
#define ABSTRACT = 0;
#define OVERRIDE override;
#else
#define VIRTUAL
#define ABSTRACT
#define OVERRIDE
#endif

//#define DEVICE_GNSS_FEATURE


class IDriver
{
  enum DeviceType
  {
    GNSS
  };

  struct MetaData
  {
    uint16_t deviceType;
    uint16_t features;
  };

public:
  VIRTUAL MetaData& getMetaData() ABSTRACT;
};
