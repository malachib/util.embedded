#include "Service.h"

using namespace FactUtilEmbedded::layer5;

void Service::doStart()
{
  setState(Starting);
  start();
  // must explicitly state a success
  if(getState() == Starting)
    setState(Error);
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
