#if defined(ESP8266)

#include "service.h"

namespace FactUtilEmbedded
{
namespace layer5
{
// this one is progmem
const char WiFiService::NAME[] PROGMEM = "WiFi";

const char* WiFiService::_WLAN_SSID;
const char* WiFiService::_WLAN_PASS;

void WiFiService::start()
{
  uint8_t retries = 10;

  setStatusMessage(F("Connecting"));

  WiFi.begin(_WLAN_SSID, _WLAN_PASS);
  // TODO: perform better status reporting
  do
  {
    auto s =  WiFi.status();
    switch(s)
    {
      case WL_CONNECTED:
        setState(Started);
        return;

      case WL_NO_SSID_AVAIL:
        setStatusMessage(F("Connecting Failed AP not found"));
        break;

      // eat these up since this is what we get while connecting, and is
      // implicit from the "Connecting" status message
      case WL_DISCONNECTED:
        break;

      default:
        setStatusMessage(F("Connecting failed"));
        break;
    }

    delay(500);
#ifdef DEBUG2
    Serial.print(F("WiFi status = "));
    Serial.println(s);
#endif
  } while(retries--);
}
}
}
#endif
