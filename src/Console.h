#pragma once

#define CONSOLE_FEATURE_AUTOCOMPLETE 1
#define CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR 1
//#define CONSOLE_FEATURE_MULTICONSOLE
//#define CONSOLE_BEHAVIOR_PROPAGATE


#include "lib.h"
#include "Menu.h"
#include <LinkedList.h>


namespace FactUtilEmbedded
{

class IMenuHandler : public util::IHandler
{

};


class IEnterable
{
protected:
  virtual void enter() = 0;
  virtual void exit() = 0;
};

// Glues I/O logic to menu (interacts with Serial, etc)
class Console : public IMenu
{
  char inputLine[128];
  uint8_t inputPos = 0;
  //Print& cout; // TODO
  //Print& cin;
public:
  char* getInputLine()
  {
    // we don't normally keep uninitialized values zeroed out, so do it here
    inputLine[inputPos] = 0;
    return inputLine;
  }

  // pos = pos within input line to copy to
  void appendToInputLine(char* src)
  {
    while(*src != 0) inputLine[inputPos++] = *src++;
  }

  uint8_t getInputPos() { return inputPos; }

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


// Special menu item which in turn can handle enumeration of menu items
class MenuHandler : public Menu, public MenuEnumerator
{
protected:
    virtual void handleCommand(Parameters p) override;
    virtual Menu* canHandle(Parameters p) override;

    void showHelp(Parameters p);

public:
#if defined(CONSOLE_FEATURE_AUTOCOMPLETE) && defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)
  virtual bool processInput(Console* console, char received) override;
#endif
};


// More C++-ish version of ConsoleMenuDef.
// this class specifically exists to handle breadcrumb behavior.  Menu behavior itself
// is managed at the MenuHandler level
class ConsoleMenuHandler : public Console
{
  IMenu* breadCrumb[4];
  uint8_t breadCrumbPos = 0;

#if defined(CONSOLE_FEATURE_AUTOCOMPLETE) && defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)
  virtual bool processInput(Console* console, char received) override;
#endif

protected:
  virtual void handleCommand(Parameters p) override;
  virtual void showPrompt() override;

public:
  ConsoleMenuHandler(IMenu* rootMenu)
  {
    breadCrumb[0] = rootMenu;
    breadCrumbPos = 1;
  }

  IMenu* getActiveMenu() { return breadCrumb[breadCrumbPos - 1]; }
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
