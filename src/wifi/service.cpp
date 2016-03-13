#if defined(MQTT_TYPE_WIFI) && defined(ESP8266)

#include "service.h"

bool WiFi_Service::start(Service& svc)
{
  uint8_t retries = 10;

  svc.setStatusMessage(F("Connecting"));

  WiFi.begin(_WLAN_SSID, _WLAN_PASS);
  // TODO: get better status reporting once non-const setStatusMessage
  // is available
  do
  {
    auto s =  WiFi.status();
    switch(s)
    {
      case WL_CONNECTED:
        return true;

      case WL_NO_SSID_AVAIL:
        svc.setStatusMessage(F("Connecting Failed AP not found"));
        break;

      // eat these up since this is what we get while connecting, and is
      // implicit from the "Connecting" status message
      case WL_DISCONNECTED:
        break;

      default:
        svc.setStatusMessage(F("Connecting failed"));
        break;
    }

    delay(500);
#ifdef DEBUG2
    Serial.print(F("WiFi status = "));
    Serial.println(s);
#endif
  } while(retries--);

  return false;
}

#endif
