#pragma once

namespace FactUtilEmbedded
{
  // layer1 = direct memory to a value, not ptr/ref involved
  namespace layer1
  {
    template <class T>
    class Property
    {
    private:
      T value;

    public:
      // FIX: this constructor introduces an inconsistency because
      // it takes a ref but the other layers take a pointer -
      // sensible but it makes constructing these generically tricky -
      // so consider making others take a ref also
      Property(const T& value) : value(value) {}

      const T& get() const { return value; }
      void set(const T& _value) { value = _value; }

      operator const T&() const { return get(); }
      void operator=(const T& _value) { set(_value); }
    };
  }

  // layer2 = no frills pointer to a type
  namespace layer2
  {
    // note this differs subtly but importantly from layer1 -
    // layer1 returns a const T& get and also doesn't switch
    // back and forth from ref to ptr
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
#ifdef PROPERTY_FEATURE_VALIDATE
      virtual PGM_P validate(const T& value) = 0;
#endif

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

#ifdef PROPERTY_FEATURE_VALIDATE
      virtual PGM_P validate(const T& value) override { return nullptr; }
#endif
    };
  }
}
