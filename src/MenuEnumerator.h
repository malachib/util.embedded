#pragma once

#include "fact/Menu.h"

namespace FactUtilEmbedded
{
  class MenuCommand;
  class MenuGeneric;
  
  class MenuEnumerator
  {
    SinglyLinkedList menus;

  protected:
    void add(MenuCommand& menu);
    void add(MenuCommand& menu, const __FlashStringHelper* name, const __FlashStringHelper* description);
    void add(MenuGeneric& menu, const __FlashStringHelper* name, const __FlashStringHelper* description, menuHandler handler)
    {
      add(menu, name, description);
      menu.setHandler(handler);
    }

    MenuCommand* canHandle(IMenu::Parameters p);
    SinglyLinkedNode* getHeadMenu() { return menus.getHead(); }
  };
}
