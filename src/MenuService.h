#pragma once

#include "Service.h"
#include "fact/Menu.h"

#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC

namespace FactUtilEmbedded
{

class MenuService : public Menu
{
  Service& service;

protected:
  virtual void handleCommand(Parameters p) override;

public:
  MenuService(Service& service) : service(service) {}
  MenuService(Service& service, startServiceInvoker invoker);

};

}

#endif
