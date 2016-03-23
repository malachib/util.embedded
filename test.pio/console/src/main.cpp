// exists to address GitHub Issue#3

#include <Arduino.h>

#include <fact/Menu.h>
#include <fact/lib.h>
#include <MenuService.h>
#include <Console.h>
#include <Service.h>

using namespace util;

#include "DummyMenu.h"

void dummyHandler(IMenu::Parameters p);

Service svc1;

MenuGeneric menuGeneric2(dummyHandler);
MenuService menuService(svc1);

DummyMenu menu;
ConsoleMenu console(&menu);

void dummyHandler(IMenu::Parameters p)
{
  menu.testHelp();
}



void setup()
{
  Serial.begin(115200);
  Serial << F("Starting up");
  Serial.println();

  //menu.getHeadMenu();
}


void loop()
{
  console.handler();
}
