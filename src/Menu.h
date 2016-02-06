#pragma once

namespace FactUtilEmbedded
{
  class Console;
  class MenuHandler;
  class ConsoleMenuHandler;

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
    friend ConsoleMenuHandler;
    friend MenuHandler;

  protected:
    virtual void showPrompt() = 0;
    virtual void handleCommand(Parameters p) = 0;
  #ifdef CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR
    // return value of true means input was processed and needs no further processing.
    // note that process does not mean command executed, but only that the one character
    // was handled in a specific way (i.e. tab completion)
    virtual bool processInput(Console* console, char c) { return false; }
  #endif

    static void showKeyValuePair(const __FlashStringHelper* key, const __FlashStringHelper* value, uint8_t keyPadding);
  };
}
