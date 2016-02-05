#pragma once

#include "lib.h"
#include <LinkedList.h>

namespace FactUtilEmbedded
{

class IMenuHandler : public util::IHandler
{

};

class ConsoleMenuHandler;

class IMenuBase
{
public:
  struct Parameters
  {
    char** parameters;
    uint8_t count;

    Parameters(char** parameters, int count) :
      parameters(parameters), count(count) {}

    Parameters inc()
    {
      Parameters p(parameters + 1, count - 1);

      return p;
    }
  };
};

class IMenu : public IMenuBase
{
  friend ConsoleMenuHandler;

protected:
  virtual void showPrompt() = 0;
  virtual void handleCommand(Parameters p) = 0;
};

// Glues I/O logic to menu (interacts with Serial, etc)
class Console : public IMenu
{
  char inputLine[128];
  uint8_t inputPos = 0;
  //Print& cout; // TODO
  //Print& cin;

public:

public:
  void handler();
  bool handler(char** parameters, int count, PGM_P keyword, void (Console::*func)(void));
};


class MenuBase
{
protected:
  // name & description are always PROGMEM residents
  const __FlashStringHelper* name;
  const __FlashStringHelper* description;

  MenuBase() {}

public:
  MenuBase(const __FlashStringHelper* name, const __FlashStringHelper* description) :
    name(name), description(description)
  {
    //this->name = (const char*) name;
    //this->description = (const char*) description;
  }

  MenuBase(const char* name, const char* description) :
    name((const __FlashStringHelper*) name),
    description((const __FlashStringHelper*) description)
  {

  }

  const __FlashStringHelper* getName() { return name; }
  const __FlashStringHelper* getDescription() { return description; }
  /*
  bool canHandle(Console::Parameters p)
  {
    return strcmp_P(p.parameters[0], (const char*) name) == 0;
  }*/
};



class Menu :
  public MenuBase,
  public IMenu,
  public util::IHandler2<IMenu::Parameters, IMenu*>,
  public util::SinglyLinkedNode
{
protected:
  virtual void showPrompt() override;

  Menu() {}

public:
  Menu(const __FlashStringHelper* name, const __FlashStringHelper* description) :
    MenuBase(name, description) {}

  Menu(const char* name, const char* description) :
    MenuBase(name, description) {}

  virtual IMenu* canHandle(IMenu::Parameters input) override;
};

typedef void (*menuHandler)(IMenu::Parameters);


// We allow MenuGeneric some deferred initialization because Global initialization
// doesn't work well for PSTR and anonymous function pointers
class MenuGeneric : public Menu
{
  menuHandler handler;

protected:
  virtual void handleCommand(Parameters p) override;

public:
  MenuGeneric() {}

  MenuGeneric(menuHandler handler) :
    Menu(name, description) { this->handler = handler; }

  void setDesc(const __FlashStringHelper* name, const __FlashStringHelper* description)
  {
    this->name = name;
    this->description = description;
  }

  void setHandler(menuHandler handler) { this->handler = handler; }
};

class MenuEnumerator
{
  SinglyLinkedList menus;

protected:
  void add(Menu& menu);
  void add(MenuGeneric& menu, const __FlashStringHelper* name, const __FlashStringHelper* description);
  void add(MenuGeneric& menu, const __FlashStringHelper* name, const __FlashStringHelper* description, menuHandler handler)
  {
    add(menu, name, description);
    menu.setHandler(handler);
  }

  Menu* canHandle(IMenu::Parameters p);
  SinglyLinkedNode* getHeadMenu() { return menus.getHead(); }
};



// More C++-ish version of ConsoleMenuDef
class ConsoleMenuHandler : public Console, public MenuEnumerator
{
  IMenu* breadCrumb[4];
  uint8_t breadCrumbPos = 0;

protected:
  virtual void handleCommand(Parameters p) override;
  virtual void showPrompt() override;

  void showHelp(Parameters p);
};



}

// temporary until we fully enable this inside Console class itself
#ifdef SAML_SERIES
#define cout SerialUSB
#define cin SerialUSB
#else
#define cout Serial
#define cin Serial
#endif
