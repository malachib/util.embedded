#pragma once

#include "lib.h"

class IMenuHandler : public util::IHandler
{

};

class ConsoleMenuHandler;

class IMenu
{
  friend ConsoleMenuHandler;

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

protected:
  virtual void showPrompt() = 0;
  virtual void handleCommand(Parameters p) = 0;
};

class Menu : public IMenu
{

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

// More C++-ish version of ConsoleMenuDef
class ConsoleMenuHandler : public Console
{
  IMenu* breadCrumb[4];
  uint8_t breadCrumbPos = 0;

protected:
  virtual void handleCommand(Parameters p) override;
  virtual void showPrompt() override;
};




// temporary until we fully enable this inside Console class itself
#ifdef SAML_SERIES
#define cout SerialUSB
#define cin SerialUSB
#else
#define cout Serial
#define cin Serial
#endif
