#include "MenuService.h"

#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC

namespace FactUtilEmbedded
{

void MenuService::handleCommand(Parameters p)
{
  service.restart();
}
/*
typedef void (Service::*_fn)();

void MenuServiceRPC::handleCommand(Parameters p)
{
  _fn fn = &Service::restart;
  auto callHolder = rpc::CallHolderFactory::create(fn, &service);
  cq.put(callHolder, sizeof(callHolder));
} */
}

#endif
