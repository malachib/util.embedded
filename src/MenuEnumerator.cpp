#include <Arduino.h>

#include "MenuEnumerator.h"

namespace FactUtilEmbedded
{


void MenuEnumerator::add(MenuCommand& menu)
{
  menus.add(&menu);
}

void MenuEnumerator::add(MenuCommand& menu, const __FlashStringHelper* name, const __FlashStringHelper* description)
{
  menu.setName(name);
  menu.setDescription(description);
  //menu.setDesc(name, description);
  menus.add(&menu);
}

MenuCommand* MenuEnumerator::canHandle(IMenu::Parameters p)
{
#ifdef DEBUG2
  cout.println("MenuEnumerator::canHandle");
#endif

  MenuCommand* menu = (MenuCommand*) menus.getHead();

  while(menu != NULL)
  {
    #ifdef DEBUG2
      cout.println("MenuEnumerator::canHandle 1");
    #endif

    if(menu->canHandle(p))
      return menu;

    menu = (MenuCommand*) menu->getNext();
  }

#ifdef DEBUG2
  cout.println("MenuEnumerator::canHandle exit");
#endif

  return NULL;
}
}
