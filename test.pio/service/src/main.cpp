#include <Arduino.h>
#include <wifi/service.h>

#include "secrets.h"

using namespace util;

layer5::WiFiService wifiService;

void statusUpdated(layer3::ServiceBase* s, const char* extra)
{
  Serial << F("Status updated: ") << s->getStatusMessage();
  if(extra != nullptr) Serial << F(", ") <<  extra;
  Serial.println();
}


void stateUpdated(layer3::ServiceBase* s)
{
  // TODO: I think I'd prefer getStateString to be static and take getState()
  // as an input.  Or maybe a cast operator from the enum to (const __FlashString*)
  Serial << F("State updated: ") << s->getStateString();
  Serial.println();

  if(s->isStarted())
  {
    Serial << F("IP Address");
    Serial.println(WiFi.localIP());
  }
}

// TODO: Figure out if we want to commit ourselves to a blocking or nonblocking
// service model
void setup()
{
  delay(3000);
  Serial.begin(115200);

  wifiService.statusUpdated += statusUpdated;
  wifiService.stateUpdated += stateUpdated;

  // TODO: unify start/doStart external access before merging
  // back into dev/master
  wifiService.doStart(WLAN_SSID, WLAN_PASS);
}


void loop()
{
  static uint16_t count = 0;

  delay(1000);
  Serial << F("Loop: ") << ++count;
  Serial.println();
}
