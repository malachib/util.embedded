#pragma once
//static void mqtt_connect(Adafruit_MQTT& mqtt);

/*
template <class TMQTT>
class MQTTService //: public LightweightService
{
  void enforceBaseClass()
  {
    Adafruit_MQTT* assignMustSucceed = &mqtt;
  }
public:
  MQTTService()
  {
    //static_assert(std::is_base_of<Adafruit_MQTT, TMQTT>::value, "Derived not derived from BaseClass");
  }

  void connect() { mqtt_connect(mqtt); }

  static TMQTT mqtt;
};

extern MQTTService<Adafruit_MQTT_Client> mqtt_service;
*/
//#define MQTT_TYPE_FONA    0
//#define MQTT_TYPE_WIFI    1

//extern Adafruit_MQTT_Client mqtt;

#ifdef MQTT_TYPE_WIFI
#ifdef ESP8266
#include <ESP8266WiFi.h>
extern WiFiClient wifiClient;
#endif
#ifndef INET_CLIENT
#define INET_CLIENT wifiClient
#endif
#define MQTT_CLIENT Adafruit_MQTT_Client
#elif defined(MQTT_TYPE_FONA)
#ifndef INET_CLIENT
#define INET_CLIENT fona
#endif
#define MQTT_CLIENT Adafruit_MQTT_FONA
#else
#error Please specify MQTT_TYPE_WIFI or MQTT_TYPE_FONA
#endif

#ifdef MQTT_CLIENT

#include <Adafruit_MQTT_Client.h>
#include "../Service.h"
#include "../fact/lib.h"
#include "../wifi/service.h"
#include "../MenuService.h" // just to ensure MenuService is defined properly

extern const char PROGMEM SVC_MQTT_NAME[];

class MQTTService
{
public:
  static void connect();
};

class MQTT_Service : public util::Service
{
public:
  MQTT_Service(WiFi_Service* dependsOn = NULL) : Service(SVC_MQTT_NAME
#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC
    , MQTT_Service::setup
#endif
  )
  {
    this->dependsOn = dependsOn;
  }

  static bool setup(Service&);
  void stop();
};


extern MQTT_CLIENT mqtt;
//extern MQTTService mqtt_service;

#define SVC_SETSTATUS(msg) service.setStatusMessage(F(msg))


#endif
