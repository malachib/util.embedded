#pragma once

#include "../features.h"

#ifdef DRIVER_FEATURE_VTABLE
#define VIRTUAL virtual
#define ABSTRACT = 0;
#define OVERRIDE override
#else
#define VIRTUAL
#define ABSTRACT
#define OVERRIDE
#endif



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

template <class T>
class IInstanceFactory
{
public:
  VIRTUAL uint16_t getInstanceSize() ABSTRACT;
  VIRTUAL T* newInstance(void* instance, ...) ABSTRACT;
};

template <class TInterface, class TClass>
class IInstanceFactoryImpl : public IInstanceFactory<TInterface>
{
public:
  VIRTUAL uint16_t getInstanceSize() OVERRIDE
  {
    return sizeof(TClass);
  }

  VIRTUAL TInterface* newInstance(void* instance, ...) OVERRIDE
  {
    va_list args;
    return new (instance) TClass(args);
  }
};

#define FACTORY_NEW(factory, ...) factory.newInstance(alloca(factory.getInstanceSize()), ## __VA_ARGS__)

class DriverManager
{
  //SinglyLinkedList drivers;

public:
  void add(IDriver& driver);
};

// placement syntax not supported out of the box for Arduino
// but this article helps us out:
// http://arduino.stackexchange.com/questions/1484/allocate-object-memory-statically-intialize-it-dynamically
inline void* operator new(size_t size, void* ptr)
{
    return ptr;
}
