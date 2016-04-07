// exists to address GitHub Issue#3

#include <Arduino.h>

#include <fact/ipc.h>
#include <fact/Menu.h>
#include <fact/lib.h>
#include <MenuService.h>
#include <Console.h>
#include <Service.h>

using namespace util;

#include "DummyMenu.h"

void dummyHandler(IMenu::Parameters p);

Service svc1;

MenuService menuService(svc1);

DummyMenu menu;
ConsoleMenu console(&menu);

union TEST
{
  // we use unsigned char because it allows the crazy casting we want to do
  unsigned char* buffer[64];
  //IInvoker invoker; // can't do this because IInvoker is an abstract class, but in effect we are doing this
} _TEST;

void dummyHandler(IMenu::Parameters p)
{
  menu.testHelp();
}


void testFunc1(int value)
{
  cout << F("I got it: ") << value;
  cout.println();
}


void testInvoker()
{
  //reinterpret_cast<IInvoker*>(&_TEST.buffer)->invoke();
  ((IInvoker*)&_TEST.buffer)->invoke();
}


void setup()
{
  Serial.begin(115200);
  Serial << F("Starting up");
  Serial.println();
  
  
  auto f = IPCHelper::create(testFunc1, 3);
  memcpy(_TEST.buffer, &f, sizeof(f));
  //memcpy(_TEST.buffer, &f, sizeof(f));

  //menu.getHeadMenu();
}


void loop()
{
  console.handler();
}
