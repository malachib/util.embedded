#include "catch.hpp"

#include <iostream>
#include <cstdarg>

#include "Service.h"

using namespace util;

#define STATUS_MSG "Coming along"

int _counter = 0;

bool testServiceStart(Service& svc)
{
  svc.setStatusMessage(STATUS_MSG);
  return true;
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
    statusMsg = "Starting";
    REQUIRE(statusMsg == (const char*) svc->getStatusMessage());
      break;

    case 2:
      statusMsg = STATUS_MSG;
      REQUIRE(statusMsg == (const char*) svc->getStatusMessage());
      break;

    case 3:
      statusMsg = "Started";
      REQUIRE(statusMsg == (const char*) svc->getStatusMessage());
      break;
  }
}

SCENARIO( "Service class tests", "[services]" )
{
  GIVEN("A regular non-lightweight service")
  {
    Service service("Test Service", testServiceStart);

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
}
