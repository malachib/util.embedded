#if defined(MQTT_TYPE_WIFI) && defined(ESP8266)

#include "service.h"

bool WiFi_Service::start(Service& svc)
{
  svc.setStatusMessage(F("Connecting"));

  WiFi.begin(_WLAN_SSID, _WLAN_PASS);
  // TODO: get timeout working
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef DEBUG
    Serial.print(F("."));
#endif
  }

  return true;
}

#endif
