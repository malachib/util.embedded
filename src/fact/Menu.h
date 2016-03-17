#pragma once

#include "features.h"
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
}
