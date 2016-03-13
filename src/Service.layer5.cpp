#include "Service.h"

using namespace FactUtilEmbedded::layer5;

void Service::doStart()
{
  //setState(Starting);
  start();
}


void Service::doStop()
{
  stop();
}


void Service::doRestart()
{
  doStop();
  doStart();
}
