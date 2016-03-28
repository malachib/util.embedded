#pragma once

#include "fact/lib.h"
#include "fact/Menu.h"
#include "LinkedList.h"
#include "Console.Stream.h"
#include "MenuEnumerator.h"

#if MEMORY_OPT_DATA
#define CONSOLE_INPUTLINE_MAX 40
#else
#define CONSOLE_INPUTLINE_MAX 80
#endif

#if defined(CONSOLE_FEATURE_COUT) and not defined(CONSOLE_FEATURE_CIN)
#define CONSOLE_FEATURE_COUT_ONLY
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
protected:
#ifdef CONSOLE_FEATURE_COUT
  Stream& out;
#ifndef CONSOLE_FEATURE_CIN
#define in cin
#else
  Stream& in;
#endif
#endif
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

  void appendToInputLine_P(PGM_P src);

  uint8_t getInputPos() { return inputPos; }

public:
  void handler();
  bool handler(char** parameters, int count, PGM_P keyword, void (Console::*func)(void));

#ifdef CONSOLE_FEATURE_COUT_ONLY
  Console(Stream& out) : out(out) {}

  Stream& getOut() const { return out; }
#else
  static Stream& getOut() { return cout; }
#endif
};



// this class specifically exists to handle breadcrumb behavior.  Menu behavior itself
// is managed at the Menu/IMenu level
class ConsoleMenu : public Console
{
  IMenu* breadCrumb[4];
  uint8_t breadCrumbPos;
#ifdef CONSOLE_FEATURE_AUTOCOMPLETE_COMMAND
  uint8_t commandPos = 0;
#endif

#if defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)
  virtual bool processInput(Console* console, char received) override;
#endif

protected:
  virtual void handleCommand(Parameters p) override;
  virtual void showPrompt(Console* console) override;

public:
  ConsoleMenu(IMenu* rootMenu
#ifdef CONSOLE_FEATURE_COUT_ONLY
    , Stream& out = cout) : Console(out)
#else
    )
#endif
  {
    breadCrumb[0] = rootMenu;
    breadCrumbPos = 1;
  }

  IMenu* getActiveMenu() { return breadCrumb[breadCrumbPos - 1]; }
};



}
