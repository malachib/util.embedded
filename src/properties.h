#pragma once

namespace FactUtilEmbedded
{
  // layer2 = no frills pointer to a type
  namespace layer2
  {
    template <class T>
    class Property
    {
    private:
      T* value;

    public:
      Property(T* const value) : value(value) {}

      T& get() const { return *value; }
      void set(const T& _value) { *value = _value; }

      operator T&() const { return get(); }
      void operator=(const T& _value) { set(_value); }
    };
  }

  // layer3 = catch update events
  namespace layer3
  {
    template <class T>
    class Property : public layer2::Property<T>
    {
      // TODO: place event firing code in here once new event code
      // is imported from services branch
    public:
      Property(T* const value) : layer2::Property<T>(value) {}

      //void set(const T& value) {  }

      void operator=(const T& value)
      { layer2::Property<T>::set(value); }
    };
  }

  // layer5 = polymorphic layer
  namespace layer5
  {
    template <class T>
    class IProperty
    {
    public:
      virtual T& get() = 0;
      virtual void set(const T& value) = 0;

      operator T&() { return get(); }
      void operator=(const T& value) { set(value); }
    };

    template <class T>
    class Property : public IProperty<T>
    {
      layer3::Property<T> prop;

    public:
      Property(T* const value) : prop(value) {}

      virtual T& get() override { return prop.get(); }
      virtual void set(const T& value) override { prop.set(value); }
      // somehow this doesn't carry forward from IProperty, so redefine it here
      void operator=(const T& value) { set(value); }
    };
  }
}
