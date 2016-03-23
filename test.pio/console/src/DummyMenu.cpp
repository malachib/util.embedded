#include <fact/Menu.h>

using namespace util;

#include "DummyMenu.h"

MenuGeneric menuGeneric1(dummyHandler);


DummyMenu::DummyMenu()
{
  add(menuGeneric1, F("generic1"), F("desc for generic1"));
  //add(menuGeneric2, F("generic2"), F("desc for generic2"));
  //add(menuService, F("svc1"), F("desc for svc1"));
}

void DummyMenu::testHelp()
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
