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

class IConsole : public IMenuBase
{
  friend Menu;

protected:
  virtual void showPrompt() = 0;
  // return value of true means input was processed and needs no further processing.
  // note that process does not mean command executed, but only that the one character
  // was handled in a specific way (i.e. tab completion)
#ifdef CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR
  virtual
#endif
  bool processInput(char c) { return false; }
};


// Glues I/O Stream to some kind of action processor
class Console : public IConsole
{
  char inputLine[CONSOLE_INPUTLINE_MAX];
  uint8_t inputPos = 0;
protected:
#ifdef CONSOLE_FEATURE_COUT
#if defined(FEATURE_IOSTREAM) || defined(FEATURE_IOSTREAM_SHIM)
  ostream& out;
#else
  Stream& out;
#endif
#ifndef CONSOLE_FEATURE_CIN
#define in cin
#else
#ifdef CONSOLE_FEATURE_IOSTREAM
    istream& in;
#else
    Stream& in;
#endif
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

  uint8_t getInputPos() const { return inputPos; }

public:
  void handler();
  //bool handler(char** parameters, int count, PGM_P keyword, void (Console::*func)(void));

#ifdef CONSOLE_FEATURE_COUT_ONLY
  Console(fact_ostream& out) : out(out) {}

  fact_ostream& getOut() const { return out; }
#else
  static fact_ostream& getOut() { return cout; }
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
  virtual bool processInput(char received) override;
#endif

protected:
  virtual void handleCommand(Parameters p) override;
  virtual void showPrompt() override;

public:
  ConsoleMenu(IMenu* rootMenu
#ifdef CONSOLE_FEATURE_COUT_ONLY
    , fact_ostream& out = cout) : Console(out)
#else
    )
#endif
  {
    breadCrumb[0] = rootMenu;
    breadCrumbPos = 1;
  }

  IMenu* getActiveMenu() const { return breadCrumb[breadCrumbPos - 1]; }
};



}
