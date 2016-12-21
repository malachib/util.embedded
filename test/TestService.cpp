#include "catch.hpp"

#include <iostream>
#include <cstdarg>

#include "Service.h"
#include <fact/iostream_arduino.h>

using namespace util;

char STATUS_MSG[] = "Coming along";
//#define STATUS_MSG "Coming along"

int _counter = 0;

template <class TService>
bool _testServiceStart(TService& svc)
{
  svc.setStatusMessage((const __FlashStringHelper*)STATUS_MSG);
  return true;
}

bool testServiceStart_layer1()
{
  return false;
}

class Service_layer5 : public layer5::Service
{
public:
  Service_layer5() : Service("Test Service") {}

  virtual void start() override;
};


void Service_layer5::start()
{
  setStatusMessage((const __FlashStringHelper*)STATUS_MSG);

  // for layer5 Services, we assume success.  If there's a failure, call an
  // explicit setState & setStatusMessage
}

void statusUpdated(Service* svc)
{
#if DEBUG2
  printf("\r\nstate: %s", svc->getStatus());
  printf("\r\nstatus updated: %s\r\n", svc->getStatusMessage());
#endif
  INFO((char*)svc->getStatusMessage());
  _counter++;
  std::string statusMsg;
  switch(_counter)
  {
    case 1:
    statusMsg = SERVICE_STATUS_STARTING;
    REQUIRE(statusMsg == (const char*) svc->getStatusMessage());
      break;

    case 2:
      statusMsg = STATUS_MSG;
      REQUIRE(statusMsg == (const char*) svc->getStatusMessage());
      break;

    case 3:
      statusMsg = SERVICE_STATUS_STARTED;
      REQUIRE(statusMsg == (const char*) svc->getStatusMessage());
      break;
  }
}

SCENARIO( "Service class tests", "[services]" )
{
  GIVEN("A regular non-lightweight service")
  {
    Service service("Test Service", _testServiceStart);

    service.statusUpdated += statusUpdated;

    std::string statusMsg = "Started";

    WHEN("Starting the service")
    {
      service.start();

      REQUIRE(_counter == 3);
      REQUIRE(statusMsg == (const char*) service.getStatusMessage());
      //REQUIRE(statusMsg == )
    }
    WHEN("Stopping the service")
    {
      //
    }
  }
  GIVEN("A layer1 service")
  {
    layer1::Service<testServiceStart_layer1> service("Test Service");

    std::string state((const char*) service.getStateString());

    REQUIRE(state == SERVICE_STATUS_UNSTARTED);

    service.start();

    state = (const char*) service.getStateString();
    REQUIRE(state == SERVICE_STATUS_ERROR);
  }
  GIVEN("A layer2 service")
  {
    layer2::Service<_testServiceStart> service("Test Service");

    service.start();

    std::string status = (const char*) service.getStatusMessage();

    REQUIRE(status == STATUS_MSG);
  }
  GIVEN("A layer3 service")
  {
    layer3::Service service("Test Service", _testServiceStart);

    service.start();

    std::string status = (const char*) service.getStatusMessage();

    REQUIRE(status == STATUS_MSG);
  }
  GIVEN("A layer5 service")
  {
    Service_layer5 service;

    service.start();

    std::string status = (const char*) service.getStatusMessage();

    REQUIRE(status == STATUS_MSG);
  }
}
