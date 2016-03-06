#pragma once

#include "fact/lib.h"
#include "fact/Menu.h"
#include "LinkedList.h"

#if MEMORY_OPT_DATA
#define CONSOLE_INPUTLINE_MAX 40
#else
#define CONSOLE_INPUTLINE_MAX 80
#endif

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
  char inputLine[CONSOLE_INPUTLINE_MAX];
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



class MenuEnumerator
{
  SinglyLinkedList menus;

protected:
  void add(Menu& menu);
  void add(Menu& menu, const __FlashStringHelper* name, const __FlashStringHelper* description);
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

class NestedMenuHandler : MenuHandler
{
  // represents menu which has been 'cd'd into , or NULL if none has been
  IMenu* selected;

protected:
    IMenu* getSelected() { return selected; }

    virtual void handleCommand(Parameters p) override;
    virtual void showPrompt() override;
public:
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

#ifdef CONSOLE_STREAM
#define cout CONSOLE_STREAM
#define cin CONSOLE_STREAM
#else
// temporary until we fully enable this inside Console class itself
#ifdef SAML_SERIES
#define cout SerialUSB
#define cin SerialUSB
#else
#define cout Serial
#define cin Serial
#endif
#endif

#ifdef CONSOLE_STREAM_EXTERN
// FIX: pretty kludgey
#if CONSOLE_STREAM_EXTERN == SoftwareSerial
#include <SoftwareSerial.h>
#endif
extern CONSOLE_STREAM_EXTERN cout;
#endif
