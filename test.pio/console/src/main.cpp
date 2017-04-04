// exists to address GitHub Issue#3

#include <Arduino.h>

#include <MenuService.h>
#include <Console.h>
#include <Service.h>

#include "features.h"
#include "DummyMenu.h"

#include <fact/iostream.h>

namespace FactUtilEmbedded { namespace std {

#ifdef __AVR__
ostream cout(Serial);
istream cin(Serial);
#else
ostream cout(SerialUSB);
istream cin(SerialUSB);
#endif
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
#ifdef __AVR__
  Serial.begin(115200);
#else
    SerialUSB.begin(115200);
#endif
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
