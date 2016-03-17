#pragma once

namespace FactUtilEmbedded
{
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
