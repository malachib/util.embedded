// exists to address GitHub Issue#3

#include <Arduino.h>

#include <MenuService.h>
#include <Console.h>
#include <Service.h>

#include "features.h"
#include "DummyMenu.h"

#include <fact/iostream.h>

namespace FactUtilEmbedded { namespace std {

ostream cout(Serial);
istream cin(Serial);
ostream& clog = cout;

} }

using namespace util::std;

void dummyHandler(IMenu::Parameters p);

Service svc1;

MenuService menuService(svc1);

DummyMenu menu;
ConsoleMenu console(&menu);

void dummyHandler(IMenu::Parameters p)
{
  menu.testHelp();
}

void setup_menuProperty();

void setup()
{
  Serial.begin(115200);
  clog << F("Starting up") << endl;

#if FEATURE_MENUPROPERTY >= 3
  setup_menuProperty();
#endif
  //menu.getHeadMenu();
}


void loop()
{
  console.handler();
}
