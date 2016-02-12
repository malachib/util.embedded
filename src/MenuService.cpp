#include "MenuService.h"

#ifdef SERVICE_FEATURE_RETAINED_STARTFUNC

namespace FactUtilEmbedded
{

void MenuService::handleCommand(Parameters p)
{
  service.restart();
}

MenuService::MenuService(Service& service, startServiceInvoker invoker) : service(service)
{
  service.setInvoker(invoker);
}

}

#endif
