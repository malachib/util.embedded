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
  //Serial.println();
  //Serial << F("DBG PHASE 3");

  int8_t ret = mqtt.connect();

  if(ret != 0)
  {
    //Serial.println(mqtt.connectErrorString(ret));
    SVC_SETSTATUS("Couldn't connect");
    return false;
  }
  else
  {
    //Serial.print(F("Connected to MQTT: Client ID = "));
    //Serial.println(MQTT_CLIENTID);
    //Serial << F("Connected to MQTT: Client ID = ") << MQTT_CLIENTID;
    //Serial.println();
    //mqttInitialized = true;
  }

  return true;
}
#endif
