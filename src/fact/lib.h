#ifndef _mal_lib_h
#define _mal_lib_h

#include "debug.h"

// Mainly for WIN32
#ifdef _DEBUG
#define DEBUG 1
#endif



#if (!CYPRESS_PSOC)
#ifndef Arduino_h
#include <Arduino.h>
#endif
// AVR toolchain already defines this
//#define AVR
#else
// Only for Cypress PSoC
#define NODUINO 1
#endif


#ifdef NODUINO
#include <stdint.h>
#else
// MS unit tests define these for arm_neon
#if !defined(MS_CPP_UNITTESTFRAMEWORK) && !defined(ARDUINO) && !defined(_UINT32_T)
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
#endif
#endif


#ifndef byte
typedef unsigned char byte;
#endif

// Since CYPRESS_PSOC is largely a non-C++ situation, it doesn't
// come stock with Print class
#if !CYPRESS_PSOC
template<class T> inline Print& operator <<(Print& obj, T arg) { obj.print(arg); return obj; }
#endif

#include "../Handler.h"
#include "CircularBuffer.h"
#include "Profiler.h"

namespace FactUtilEmbedded
{

struct IObservable
{

};

struct IObserver
{
  virtual void observe(IObservable* observable) = 0;
};

/*
class ObservableLinkedList : IObservable
{
  DoublyLinkedList<IObserver> observers;

protected:
  void notify();
};
*/

struct IContainer
{

};

// Represents a source (subject) observable array
class ObservableArray : public IObservable, IContainer
{
  IObserver** observers;
  byte length;

protected:
  void notify() { notify(this); }
  void notify(IObservable* observable);

public:
  ObservableArray(IObserver** observers, byte length) :
    observers(observers),length(length)
  { }

  ObservableArray() {}

  void add(IObserver* observer);

  void setObservers(IObserver** observers, byte length = 0)
  {
    this->observers = observers;
    if(length > 0) this->length = length;
  }
};


// represents a relay observer
// as an observer, listens for one event, then broadcasts to a list of observers
class ObserverArray : public ObservableArray,
  public IObserver
{
public:
  void observe(IObservable* subject) override;
};


struct IToggleHandler : IHandler
{
  virtual void On() = 0;
  virtual void Off() = 0;
};

struct ICyclingHandler : IHandler
{
  // think in terms of state machines:
  // return true if full operation has completed
  // return false if operation still in process
  virtual bool ProcessCycle() = 0;
};

struct IVectorHandler : IHandler
{
  // Gets fed a value which moves around based on an external algorithm
  virtual bool ProcessVector(int vector) = 0;
};


struct IFuncProvider
{
  virtual int Func(int x) = 0;
};

struct TriangleWaveProvider : IFuncProvider
{
  int Func(int x)
  {
    return 0;
  }
};


class Pin
{
  protected:
  const uint8_t pinNumber;

  public:
  Pin(uint8_t _pinNumber) : pinNumber(_pinNumber) {}
};

class DigitalPin : public Pin
{
  public:
  DigitalPin(uint8_t pinNumber) : Pin(pinNumber) {}

  void Write(uint8_t signal) const
  {
#ifndef CYPRESS_PSOC
    digitalWrite(pinNumber, signal);
#endif
  }
  void SetHigh() const
  {
#ifndef CYPRESS_PSOC
    Write(HIGH);
#endif
  }
  void SetLow() const
  {
#ifndef CYPRESS_PSOC
    Write(LOW);
#endif
  }
  inline void SetMode(uint8_t mode) const
  {
#ifndef CYPRESS_PSOC
    pinMode(pinNumber, mode);
#endif
  }
};


class AnalogPin : public Pin
{
  public:
  AnalogPin(short pinNumber) : Pin(pinNumber) {}

  int Read() const
  {
#ifndef NODUINO
    return analogRead(pinNumber);
#else
    return -1;
#endif
  }

  void Write(int value) const
  {
#ifndef NODUINO
    analogWrite(pinNumber, value);
#endif
  }
};


class Led : public DigitalPin, public IToggleHandler
{

  public:
  // Just be careful, autoInitOutput happening at global scope
  // may not be happy (happening before setup is officially called)
  Led(uint8_t pinNumber, bool autoInitOutput = true);

  Led();

  // starting to conclude you can't assume LEDs are always high or low...
  // but we'll keep rolling with it for now
#if ESP8266
  inline void On() { SetLow(); }
  inline void Off() { SetHigh(); }
#else
  inline void On() { SetHigh(); }
  inline void Off() { SetLow(); }
#endif
};


class PgmExtract
{
  //bool nearMode = true;

  protected:
    //virtual bool ShouldContinue(byte value) = 0;
    // TODO: Time to break apart Render & Continue logic
    virtual bool Render(byte value) = 0;

  public:
    void Extract(byte* input);
#ifdef PGM_FAR_AVAILABLE
    void Extract(uint_farptr_t input);
#endif
};

class PgmSerialExtract : public PgmExtract
{
  protected:
    virtual bool Render(byte value) override;
  public:
    void println(const char* input)  { Extract((byte*)input); }
};

#ifdef AVR
class PgmStream //: public PgmExtract
{
  Stream& stream;
public:
  PgmStream(Stream& _stream) : stream(_stream) {}

  void println(const char* input);
};


class PgmStringExtract : public PgmExtract
{
  char* output;

    protected:
    virtual bool Render(byte value) override;

    public:
    PgmStringExtract(char* _output) : output(_output) {}
    PgmStringExtract() {}

    char* Extract(const char* input);
    char* Extract(const char* input, char* output);
};
#endif

void pgm_extract_bytes(byte* input, byte* output, int length);
void pgm_extract_string(const char* input, char* output);
void pgm_extract_bytes(byte* input, bool (*foo)(byte,void*));

// TODO: Work out message queing
template <class T>
class ObserverMessenger : public IObserver
{
  CircularBuffer<T> messages;
protected:
  virtual T getMessage(IObservable* observable) = 0;
public:
  void observe(IObservable* observable) override;

  ObserverMessenger(T* messages, int size)
    : messages(messages, size)
    {}

  T get() { return messages.get(); }

  uint16_t available() { return messages.available(); }
};

template <class T>
void ObserverMessenger<T>::observe(IObservable* observable)
{
  T message = getMessage(observable);
  messages.put(message);
}

// TODO: integrate this with PSTR_Property
#define PROPERTY_CLASS(className, type, name) \
class className                               \
{                                             \
  type value;                                 \
\
protected:                                    \
\
  className() {}                              \
  className(type value) : value(value) {}  \
\
  void set##name(type value) { this->value = value; } \
public:                                       \
  type get##name() const { return value; } \
};

PROPERTY_CLASS(Named, const __FlashStringHelper*, Name)
PROPERTY_CLASS(Described, const __FlashStringHelper*, Description)

}

namespace Mal_Lib = FactUtilEmbedded;
namespace util = FactUtilEmbedded;

#endif
