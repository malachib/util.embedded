#pragma once

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#error Only ESP8266 supported at this time
#endif
#include "../Service.h"
#include "../fact/lib.h"
#include "../MenuService.h" // just to ensure MenuService is defined properly

extern const char PROGMEM SVC_WIFI_NAME[];
extern WiFiClient wifiClient;

class WiFi_Service : public util::Service
{
  // not progmem , just regular const char*
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
  void setSSID(const char* ssid, const char* pass)
  {
    _WLAN_SSID = ssid;
    _WLAN_PASS = pass;
  }
  void start(const char* ssid, const char* pass)
  {
    setSSID(ssid, pass);
    util::Service::start();
  }
};
