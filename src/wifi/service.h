#pragma once

#include <ESP8266WiFi.h>
#include "../Service.h"
#include "../fact/lib.h"
#include "../MenuService.h" // just to ensure MenuService is defined properly

extern const char PROGMEM SVC_WIFI_NAME[];
extern WiFiClient wifiClient;

class WiFi_Service : public util::Service
{
  static const char* _WLAN_SSID;
  static const char* _WLAN_PASS;

public:
  WiFi_Service() : Service(SVC_WIFI_NAME
#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC
    , WiFi_Service::start
#endif
  )
  {}

  static bool start(Service&);
  void stop();
};
