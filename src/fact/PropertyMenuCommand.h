#pragma once

#include <Arduino.h>
#include "MenuCommand.h"
#include "../properties.h"
#include "string_convert.h"

namespace FactUtilEmbedded
{
  extern const char PROPERTYMENUCMD_GET[] PROGMEM;
  extern const char PROPERTYMENUCMD_SET[] PROGMEM;

  // NOTE: trouble with this approach is it doesn't aggregate multiple
  // properties well
  // TODO: Upgrade this so it takes "set [NAME] [VALUE]".
  // Right now it's just "set [VALUE]"
  class PropertyMenuCommandBase : public MenuCommand
  {
  public:
#if defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)
    virtual bool processInput(Console* console, char received) override;
#endif
  };

  template <class T, template <class T> class TProperty = layer3::Property>
  class PropertyMenuCommand : public PropertyMenuCommandBase
  {
    TProperty<T> property;

  protected:
    virtual void handleCommand(Parameters p) override
    {
      if(strcmp_P(*p.parameters, PROPERTYMENUCMD_GET) == 0)
      {
#ifdef UNIT_TEST
        cout << "\r\nDISPLAY THIS: ";
#endif
        // TODO: use p.console if available
        cout << property.get();

#ifdef UNIT_TEST
        cout.println();
#endif
      }
      else if(strcmp_P(*p.parameters, PROPERTYMENUCMD_SET) == 0)
      {
        property = fromString<T>(p.parameters[1]);
      }
    }

    virtual IMenu* canHandle(Parameters p) override
    {
      // TODO: document how handleCommand and canHandle interact
      return nullptr;
    }

  public:
    PropertyMenuCommand(T* const value) : property(value) {}

#ifdef UNIT_TEST
    void _handleCommand(Parameters p) { handleCommand(p); }
#endif
  };
}
