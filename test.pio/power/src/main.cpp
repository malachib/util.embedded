#include <Arduino.h>

#include <fact/Menu.h>
#include <fact/lib.h>
#include <fact/power.h>
#include <MenuService.h>
#include <Console.h>
#include <Service.h>

using namespace util;

Service svc1;

MenuService menuService(svc1);

Menu menu;
ConsoleMenu console(&menu);


void setup()
{
  cout.begin(115200);
  cout << F("Starting up");
  cout.println();

  //menu.getHeadMenu();
}


void loop()
{
  console.handler();
}
