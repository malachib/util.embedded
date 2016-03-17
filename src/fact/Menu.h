#pragma once

#include "features.h"
#include "../Handler.h"
#include "../Console.Stream.h"
#include "lib.h"

namespace FactUtilEmbedded
{
  class Console;
  class Menu;
  class ConsoleMenu;
  class NestedMenuHandler;

  class IMenuBase
  {
  public:
    struct Parameters
    {
      char** parameters;
      uint8_t count;

  #ifdef CONSOLE_BEHAVIOR_PROPAGATE
      Console* console;
  #endif

      Parameters(char** parameters, int count, Console* console) :
        parameters(parameters), count(count)
  #ifdef CONSOLE_BEHAVIOR_PROPAGATE
        ,console(console)
  #endif
        {}

      Parameters inc()
      {
  #ifdef CONSOLE_BEHAVIOR_PROPAGATE
        Parameters p(parameters + 1, count - 1, console);
  #else
        Parameters p(parameters + 1, count - 1, NULL);
  #endif

        return p;
      }
    };
  };

  class IMenu : public IMenuBase
  {
    friend ConsoleMenu;
    friend Menu;
    friend NestedMenuHandler;

  protected:
    virtual void showPrompt() = 0;
    virtual void handleCommand(Parameters p) = 0;
    // return value of true means input was processed and needs no further processing.
    // note that process does not mean command executed, but only that the one character
    // was handled in a specific way (i.e. tab completion)
#ifdef CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR
    virtual
#endif
    bool processInput(Console* console, char c) { return false; }

    static void _showKeyValuePair(const __FlashStringHelper* key, uint8_t keyPadding);

    template <class T>
    static void showKeyValuePair(const __FlashStringHelper* key, T value, uint8_t keyPadding)
    {
      _showKeyValuePair(key, keyPadding);
      cout.print(value);
    }
  };

  class MenuEnumerator;

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

  typedef void (*menuHandler)(IMenu::Parameters);


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
