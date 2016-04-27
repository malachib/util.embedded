#pragma once

#include <Arduino.h>
#include "MenuCommand.h"
#include "../properties.h"
#include "string_convert.h"

namespace FactUtilEmbedded
{
  extern const char PROPERTYMENUCMD_GET[] PROGMEM;
  extern const char PROPERTYMENUCMD_SET[] PROGMEM;
  extern const char PROPERTYMENUCMD_GET_DESC[] PROGMEM;
  extern const char PROPERTYMENUCMD_SET_DESC[] PROGMEM;

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

#ifdef UNIT_TEST
    void _handleCommand(Parameters p) { handleCommand(p); }
#endif
  };

  template <class T, template <class T2> class TProperty = layer3::Property>
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

    // canHandle is roughly for aggregators to decide whether or not
    // to throw a handleCommand a particular direction.  It can be
    // for "this" or any other IMenu* that canHandle deems worthy -
    // although there's a descrepency between IMenu and MenuCommand which
    // needs to be worked out/documented
    virtual IMenu* canHandle(Parameters p) override
    {
      // TODO: document how handleCommand and canHandle interact
      return nullptr;
    }

  public:
    PropertyMenuCommand(T* const value) : property(value) {}
  };

  class PropertyMenu : public MenuCommand
  {
    SinglyLinkedList items;

  public:


    virtual void handleCommand(Parameters p) override
    {
      if(strcmp_P(*p.parameters, PROPERTYMENUCMD_GET) == 0)
      {

      }
      else if(strcmp_P(*p.parameters, PROPERTYMENUCMD_SET) == 0)
      {

      }
    }


    //void add(ItemBase& item) { items.add(&item); }
  };

  namespace menu
  {
    class PropertyBase : public SinglyLinkedNode, public Named
    {
    public:
      PropertyBase(const char* name) : Named((const __FlashStringHelper*) name) {}

      virtual void get(Stream& out) = 0;
      virtual void set(const char* parameter) = 0;
    };

    // This class exists so we can re-use code between the different layer::Property
    /* To do this we'd have to not just init "property" but also assign it and hope
       compiler optimizes that nicely.  Until we test to see if the compilers can
      handle that well, dup code declaration instead
    template <class TProperty>
    class ItemPropBase : ItemBase
    {
    protected:
      TProperty property;

      virtual void get(Stream& out) override
      {
        out.print(property.get());
      }

      virtual void set(const char* parameter) override
      {
        property = fromString<T>(parameter);
      }
    }; */


    template <class T, template <class T2> class TProperty = layer2::Property>
    class Property : public PropertyBase
    {
      TProperty<T> property;

    public:
      Property(const char* name, T* const value) :
        PropertyBase(name),
        property(value)
        {}

      virtual void get(Stream& out) override
      {
        out.print(property.get());
      }

      virtual void set(const char* parameter) override
      {
        property = fromString<T>(parameter);
      }
    };


    template <class T>
    class PropertyRef : public PropertyBase
    //: public ItemPropBase<layer5::IProperty<T>&>
    {
      layer5::IProperty<T>& property;

    public:
      PropertyRef(const char* name, layer5::IProperty<T>& property) :
        property(property), PropertyBase(name) {}

      virtual void get(Stream& out) override
      {
        out.print(property.get());
      }

      virtual void set(const char* parameter) override
      {
        property = fromString<T>(parameter);
      }
    };
  }

  class SetPropertyMenuCommand : public MenuCommand
  {
    const layer3::Array<menu::PropertyBase*> properties;

  public:
    SetPropertyMenuCommand(layer3::Array<menu::PropertyBase*> _properties) :
      MenuCommand(PROPERTYMENUCMD_SET, PROPERTYMENUCMD_SET_DESC),
      properties(_properties)
    {}

    virtual void handleCommand(Parameters p) override;

#ifdef UNIT_TEST
    void _handleCommand(Parameters p) { handleCommand(p); }
#endif
  };

  class GetPropertyMenuCommand : public MenuCommand
  {
    const layer3::Array<menu::PropertyBase*> properties;

  public:
    GetPropertyMenuCommand(layer3::Array<menu::PropertyBase*> _properties) :
      MenuCommand(PROPERTYMENUCMD_GET, PROPERTYMENUCMD_GET_DESC),
      properties(_properties)
    {}

    virtual void handleCommand(Parameters p) override;

#ifdef UNIT_TEST
    void _handleCommand(Parameters p) { handleCommand(p); }
#endif
  };

  // consider an "aggregate command" IMenu command which can handle
  // more than one command within itself.  Right now, Menu & MenuCommand
  // very much expect MenuCommand to map to exactly one command
  //class GetSetPropertyMenuCommand
}
