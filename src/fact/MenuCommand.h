#pragma once

#include "features.h"
#include "../Handler.h"
#include "../Console.Stream.h"
#include "lib.h"
#include "IMenu.h"

namespace FactUtilEmbedded
{
  class Menu;
  class MenuEnumerator;
  class NestedMenuHandler;

  class MenuBase :
    public SinglyLinkedNode,
    public Named,
    public Described
  {
    friend MenuEnumerator;

  protected:
    MenuBase() {}

  public:
    MenuBase(const __FlashStringHelper* name, const __FlashStringHelper* description) :
      Named(name), Described(description) {}

    MenuBase(PGM_P name, PGM_P description) :
      Named((const __FlashStringHelper*)name), Described((const __FlashStringHelper*)description) {}
  };

  class MenuCommand :
    public MenuBase,
    // see below comment, moving SinglyLinkedNode up here doesn't help
    //public SinglyLinkedNode,
    public IMenu,
    public IHandler2<IMenu::Parameters, IMenu*>
    // it appears if we use SinglyLinkedNode here, we cannot cast MenuCommand to MenuBase
    // therefore we use a shim _MenuBase .  It seems like it's VTABLE related somehow
    //,
    //public SinglyLinkedNode
  {
  protected:
    virtual void showPrompt() override;

    MenuCommand() {}

  public:
    MenuCommand(const __FlashStringHelper* name, const __FlashStringHelper* description) :
      MenuBase(name, description) {}

    MenuCommand(PGM_P name, PGM_P description) :
      MenuBase(name, description) {}

    virtual IMenu* canHandle(IMenu::Parameters input) override;
  };


  // We allow MenuGeneric some deferred initialization because Global initialization
  // doesn't work well for PSTR and anonymous function pointers
  class MenuGeneric : public MenuCommand
  {
    menuHandler handler;

  protected:
    virtual void handleCommand(Parameters p) override;

  public:
    MenuGeneric() {}

    MenuGeneric(menuHandler handler)
      //:
      //MenuCommand(name, description)
      { this->handler = handler; }

    void setHandler(menuHandler handler) { this->handler = handler; }
  };
}
