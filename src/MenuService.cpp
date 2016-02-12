#include "MenuService.h"

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
