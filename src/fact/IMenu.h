#pragma once

#include "features.h"
#include "compatibility.h"
// because of cout references in this .h file
#include "../Console.Stream.h"

namespace FactUtilEmbedded
{
  class Menu;
  class Console;
  class ConsoleMenu;
  class NestedMenuHandler;

  class IMenuBase
  {
  public:
    struct Parameters
    {
      const char** parameters;
      uint8_t count;

  #ifdef CONSOLE_BEHAVIOR_PROPAGATE
      const Console* console;
  #endif

      Parameters(const char** parameters, int count, const Console* console) :
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
    // TODO: consider breaking off an IConsole because Consoles don't technically
    // need Console* parameter on their implementation.
    virtual void showPrompt(Console* console) = 0;
    virtual void handleCommand(Parameters p) = 0;
    // return value of true means input was processed and needs no further processing.
    // note that process does not mean command executed, but only that the one character
    // was handled in a specific way (i.e. tab completion)
#ifdef CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR
    virtual
#endif
    bool processInput(Console* console, char c) { return false; }

    static void _showKeyValuePair(
#ifdef CONSOLE_FEATURE_COUT
      Stream& out,
#endif
      const __FlashStringHelper* key, uint8_t keyPadding);

    template <class T>
    static void showKeyValuePair(
      Stream& out, const __FlashStringHelper* key, T value, uint8_t keyPadding)
    {
      _showKeyValuePair(
#ifdef CONSOLE_FEATURE_COUT
        out,
#endif
        key, keyPadding);

      out.print(value);
    }
  };


  typedef void (*menuHandler)(IMenu::Parameters);
}
