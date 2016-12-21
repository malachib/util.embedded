#include "Console.h"
#include "fact/Menu.h"
#include "fact/PropertyMenuCommand.h"

namespace FactUtilEmbedded
{
    using namespace std;

  const char PROPERTYMENUCMD_GET[] PROGMEM = "get";
  const char PROPERTYMENUCMD_SET[] PROGMEM = "set";
  const char PROPERTYMENUCMD_GET_DESC[] PROGMEM = "reads a value from property";
  const char PROPERTYMENUCMD_SET_DESC[] PROGMEM = "writes a value to property";

#if defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)
  bool PropertyMenuCommandBase::processInput(Console* console, char received)
  {
    return false;
  }
#endif

  void GetPropertyMenuCommand::handleCommand(Parameters p)
  {
    MENU_DECLARE_COUT;

    uint8_t i = 0;

    if(p.count == 0)
    {
      // TODO: get a foreach style iterator going for buffers - look at std/boost
      // and copy theirs
      for(; i < properties.getSize(); i++)
      {
        auto item = properties[i];

        out << F("  ") << item->getName() << endl;
      }
    }
    else if(p.count == 1)
    {
      const char* name = *p.parameters;

      for(; i < properties.getSize(); i++)
      {
        auto item = properties[i];
        const __FlashStringHelper* _itemName = item->getName();
        auto itemName = reinterpret_cast<const char*>(_itemName);

        if(strcmp_P(name, itemName) == 0)
        {
          item->get(out);
          out << endl;
        }
      }
    }
  }



  void SetPropertyMenuCommand::handleCommand(Parameters p)
  {
    MENU_DECLARE_COUT;

    uint8_t i = 0;

    if(p.count == 2)
    {
      const char* name = p.parameters[0];
      const char* value = p.parameters[1];

      for(; i < properties.getSize(); i++)
      {
        auto item = properties[i];
        const __FlashStringHelper* _itemName = item->getName();
        auto itemName = reinterpret_cast<const char*>(_itemName);

        if(strcmp_P(name, itemName) == 0)
        {
#ifdef PROPERTY_FEATURE_VALIDATE
          PGM_P validationError = item->validate(value);

          if(validationError)
          {
            out << F("Error setting property ");
            out << _itemName << endl;
            out << reinterpret_cast<const __FlashStringHelper*>(validationError) << endl;
            return;
          }
#endif

          item->set(value);
        }
      }
    }
  }
}
