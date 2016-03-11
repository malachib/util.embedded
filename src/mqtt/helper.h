#include <Arduino.h>

#include "../Service.h"

#include "service.h"

#include <Adafruit_MQTT.h>

#ifdef MQTT_TYPE_WIFI
#ifdef ESP8266
#include <ESP8266WiFi.h>
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient wifiClient;
#endif
#endif

// Store the MQTT server, client ID, username, and password in flash memory.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;

// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[] PROGMEM  = AIO_KEY __DATE__ __TIME__;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

//#if MQTT_CONN_TYPE == "WIFI"
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
MQTT_CLIENT mqtt(&INET_CLIENT, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);
//#endif


#define MQTT_FEED(name) \
const char MQTT_FEED_#name[] PROGMEM = AIO_USERNAME "/feeds/" ##name; \
Adafruit_MQTT_Publish name = Adafruit_MQTT_Publish(&mqtt, MQTT_FEED_#name)

MQTTService mqtt_service;
