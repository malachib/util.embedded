#pragma once

#include "fact/IMenu.h"
#include "fact/lib.h"
// TODO: see if we can phase out requiring MenuCommand.h here in favor
// of the explicit placeholder classes below
#include "fact/MenuCommand.h"

namespace FactUtilEmbedded
{
  class MenuCommand;
  class MenuGeneric;

  class MenuEnumerator
  {
    SinglyLinkedList menus;

  protected:
    void add(MenuCommand& menu) { menus.add(&menu); }
    void add(MenuCommand& menu, const __FlashStringHelper* name, const __FlashStringHelper* description);
    void add(MenuGeneric& menu, const __FlashStringHelper* name, const __FlashStringHelper* description, menuHandler handler)
    {
      add(menu, name, description);
      menu.setHandler(handler);
    }

    uint16_t count() { return menus.count(); }

    MenuCommand* canHandle(IMenu::Parameters p);
    SinglyLinkedNode* getHeadMenu() { return menus.getHead(); }
    layer1::LinkedListIterator<MenuBase> getIterator() { return layer1::LinkedListIterator<MenuBase>(menus); }
  };
}
