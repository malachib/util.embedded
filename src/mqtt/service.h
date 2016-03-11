#include "Adafruit_MQTT_Client.h"

class MQTTService //: public LightweightService
{
public:
  static void connect();
};

extern MQTTService mqtt_service;
extern Adafruit_MQTT_Client mqtt;
