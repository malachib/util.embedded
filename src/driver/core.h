#pragma once

#include "../fact/features.h"
#include "../fact/c_types.h"
#include <stdarg.h>

#define FACT_VE defined(DRIVER_FEATURE_VTABLE)
#include "../fact/virtual_def.h"

class IDriver
{
public:
  enum DeviceType : uint16_t
  {
    GNSS
  };

  struct MetaData
  {
    uint16_t deviceType;
    uint16_t features = 0;
  };


  VIRTUAL MetaData getMetaData() ABSTRACT;
};

template <class T, class TParameter>
class IInstanceFactory
{
public:
  VIRTUAL uint16_t getInstanceSize() ABSTRACT;
  VIRTUAL T* newInstance(void* instance, TParameter parameter, ...) ABSTRACT;
};

template <class TInterface, class TClass, class TParameter>
class IInstanceFactoryImpl : public IInstanceFactory<TInterface, TParameter>
{
public:
  VIRTUAL uint16_t getInstanceSize() OVERRIDE
  {
    return sizeof(TClass);
  }

  // FIX: broken - can't use variadic without one formal parameter (see constructor)
  VIRTUAL TInterface* newInstance(void* instance, TParameter parameter, ...) OVERRIDE
  {
    va_list args;
    return new (instance) TClass(parameter, args);
  }
};

#define FACTORY_NEW(factory, parameter, ...) factory.newInstance(alloca(factory.getInstanceSize()), parameter, ## __VA_ARGS__)

class DriverManager
{
  //SinglyLinkedList drivers;

public:
  void add(IDriver& driver);
};

#ifdef ARDUINO
// placement syntax not supported out of the box for Arduino
// but this article helps us out:
// http://arduino.stackexchange.com/questions/1484/allocate-object-memory-statically-intialize-it-dynamically
inline void* operator new(size_t size, void* ptr)
{
    return ptr;
}
#endif

#include "../fact/virtual_undef.h"
