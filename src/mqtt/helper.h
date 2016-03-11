#include "../Service.h"

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Store the MQTT server, client ID, username, and password in flash memory.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;

// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[] PROGMEM  = AIO_KEY __DATE__ __TIME__;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);/****************************** Feeds ***************************************/

#define MQTT_FEED(name) \
const char MQTT_FEED_#name[] PROGMEM = AIO_USERNAME "/feeds/" ##name; \
Adafruit_MQTT_Publish name = Adafruit_MQTT_Publish(&mqtt, MQTT_FEED_#name)

#include "service.h"

MQTTService service;
