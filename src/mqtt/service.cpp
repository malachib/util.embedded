#if defined(MQTT_TYPE_WIFI) || defined(MQTT_TYPE_FONA)

#include "service.h"

// beware, this reaches out to Adafruit libs but project isn't really set up
// to fetch them (relying on you the programmer to have them onhand and compiler-discoverable)
void MQTTService::connect()
//void mqtt_connect(Adafruit_MQTT& mqtt)
{
  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));
}

bool MQTT_Service::setup(Service& service)//const __FlashStringHelper** status)
{
  service.setStatusMessage(F("Connecting..."));

  uint8_t retries = 5;

  do
  {
    int8_t ret = mqtt.connect();

    if(ret != 0)
    {
      // Some seem to be adafruit-specific error codes
      switch (ret)
      {
        case 1: service.setStatusMessage(F("Wrong protocol")); break;
        case 2: service.setStatusMessage(F("ID rejected")); break;
        case 3: service.setStatusMessage(F("Server unavail")); break;
        case 4: service.setStatusMessage(F("Bad user/pass")); break;
        case 5: service.setStatusMessage(F("Not authed")); break;
        case 6: service.setStatusMessage(F("Failed to subscribe")); break;
        default: service.setStatusMessage(F("Connection failed")); break;
      }

      delay(2000);
    }
    else
    {
      service.setStatusMessage(F("Connected"));
      return true;
    }

  } while(retries--);

  return false;
}

void MQTT_Service::keepAlive()
{
  if(!mqtt.ping())
  {
    setStatusMessage(F("Reconnecting"));
    mqtt.disconnect();
    setup(*this);
  }
}
#endif
