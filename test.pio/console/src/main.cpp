// exists to address GitHub Issue#3

#include <Arduino.h>

#include <fact/Menu.h>
#include <fact/lib.h>
#include <MenuService.h>
#include <Console.h>
#include <Service.h>

using namespace util;

void dummyHandler(IMenu::Parameters p);

Service svc1;

MenuGeneric menuGeneric1(dummyHandler);
MenuGeneric menuGeneric2(dummyHandler);
MenuService menuService(svc1);

class DummyMenu : public Menu
{
public:
  DummyMenu()
  {
    add(menuGeneric1, F("generic1"), F("desc for generic1"));
    //add(menuGeneric2, F("generic2"), F("desc for generic2"));
    //add(menuService, F("svc1"), F("desc for svc1"));
  }

  void testHelp()
  {
    SinglyLinkedNode* node = getHeadMenu();

    for(; node != NULL; node = node->getNext())
    {
      // NOTE: beware, casting this to MenuBase gives us the wrong getName/getDescription
      auto menu = (MenuCommand*) node;
      auto menu2 = (MenuBase*) node;

      // prints 501 consistently
      Serial << F("menu = ") << (uint32_t)menu;
      Serial.println();
      // prints 507 when deriving SinglyLinkedNode direct from MenuCommand
      // prints 503 when deriving SinglyLinkedNode direct from MenuBase
      // none of that is a red flag though
      Serial << F("menu2 = ") << (uint32_t)menu2;
      Serial.println();

      Serial << F("menu name = ") << menu->getName();
      Serial.println();
      // menu2->getName() gets unexpected pointer if deriving
      // SinglyLinkedNode direct from MenuCommand
      Serial << F("menu2 name = ") << menu2->getName();
      Serial.println();
    }
  }
};

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
