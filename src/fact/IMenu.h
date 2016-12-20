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
      const uint8_t count;

  #ifdef CONSOLE_BEHAVIOR_PROPAGATE
      const Console* console;
  #endif

      Parameters(const char** parameters, const int count, const Console* console) :
        parameters(parameters), count(count)
  #ifdef CONSOLE_BEHAVIOR_PROPAGATE
        ,console(console)
  #endif
        {}

      Parameters inc()
      {
        Parameters p(parameters + 1, count - 1,
#ifdef CONSOLE_BEHAVIOR_PROPAGATE
          console);
#else
          nullptr);
#endif
        return p;
      }
    };
  protected:
    virtual void handleCommand(Parameters p) = 0;
  };

  class IMenu : public IMenuBase
  {
    friend ConsoleMenu;
    friend Menu;
    friend NestedMenuHandler;

  protected:
    virtual void showPrompt(Console* console) = 0;
    // return value of true means input was processed and needs no further processing.
    // note that process does not mean command executed, but only that the one character
    // was handled in a specific way (i.e. tab completion)
#ifdef CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR
    virtual
#endif
    bool processInput(Console* console, char c) { return false; }

    static void _showKeyValuePair(
#ifdef CONSOLE_FEATURE_COUT
      fact_ostream& out,
#endif
      const __FlashStringHelper* key, uint8_t keyPadding);

    template <class T>
    static void showKeyValuePair(
      fact_ostream& out, const __FlashStringHelper* key, T value, uint8_t keyPadding)
    {
      _showKeyValuePair(
#ifdef CONSOLE_FEATURE_COUT
        out,
#endif
        key, keyPadding);

      out << value;
    }
  };


  typedef void (*menuHandler)(IMenu::Parameters);
}
