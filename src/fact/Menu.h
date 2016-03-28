#pragma once

#include "MenuCommand.h"
#include "../MenuEnumerator.h"

namespace FactUtilEmbedded
{
  // Special menu item which in turn can handle enumeration of menu items
  class Menu : public MenuCommand, public MenuEnumerator
  {
  protected:
      virtual void handleCommand(Parameters p) override;
      virtual MenuCommand* canHandle(Parameters p) override;

      void showHelp(Parameters p);

  public:
  #if defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)
    virtual bool processInput(Console* console, char received) override;
  #endif
  };

  class NestedMenuHandler : public Menu
  {
    // represents menu which has been 'cd'd into , or NULL if none has been
    IMenu* selected;

  protected:
      IMenu* getSelected() const { return selected; }

      virtual void handleCommand(Parameters p) override;
      virtual void showPrompt(Console* console) override;
  public:
  };
}
