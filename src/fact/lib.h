#ifndef _mal_lib_h
#define _mal_lib_h

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


template <class T>
class CircularBufferBase
{
protected:
  //T* buffer;  // still mysterious why I can't use this
};

// TODO: Do pointer arithmatic with this one.  Will take more memory but goes faster
template <class T>
class CircularBufferFast : public CircularBufferBase<T>
{
};

// TODO: make position/size etc templated also to support a byte-version of this
template <class T>
class CircularBuffer : public CircularBufferBase<T>
{
  T* buffer;
  uint16_t position;
  uint16_t position_get;
  uint16_t size;

  void incrementPositionGet()
  {
    if(++position_get >= size)
      position_get = 0;
  }

public:
  CircularBuffer<T>(T* bufferToUse, uint16_t size) : size(size)
  {
    buffer = bufferToUse;
  }

  CircularBuffer(uint16_t size) : size(size) {}

  CircularBuffer() {}

  void setBuffer(T* buffer, uint16_t size)
  {
    this->buffer = buffer;
    this->size = size;
  }

  // normally we would scope this as protected, but
  // for speed we want to be able to peekLast / incrementPosition
  // instead of copying large messages around with a regular 'put'
  void incrementPosition()
  {
    if(++position >= size)
      position = 0;
  }



  //T get();
  // TODO: See if template classes really *always* generate inline functions
  // TODO: turn this into a byte oriented version, then templatize the child class so that
  // we aren't generating inline functions all over town
  T get()
  {
    const T& value = peek();
    incrementPositionGet();
    return value;
  }

  const T& peek() { return buffer[position_get]; }

  // Returns empty, freshly available buffer slot for a put
  // mainly useful for fast-writing to circular buffer manually vs
  // a full put operation
  T& peekLast() { return buffer[position]; }

  // acquire how many elements are available to be read
  uint16_t available()
  {
    if(position_get <= position)
    {
      return position - position_get;
    }
    else
    {
      return ((size - 1) - position_get) + position;
    }
  }

  void put(T value);

  void reset()
  {
    position = 0;
    position_get = 0;
  }

  void debugPrint()
  {
#if defined(ARDUINO) && defined(DEBUG)
    for(int i = 0; i < size; i++)
    {
      Serial << buffer[i] << F(", ");
    }
#endif
  }
};


template <class T>
void CircularBuffer<T>::put(T value)
{
  buffer[position] = value;
  incrementPosition();
}


const uint16_t profileCount = 50;

class Profiler
{
  uint16_t iterator; // how many times Profile() was called before we get iterator results, which then clears it
  bool overflowed;
  uint32_t lastTimestamp;
  uint16_t intervals[profileCount];
  uint8_t frequency; // how often to actually record the interval, so that we can stretch profiling further over time without upping profileCount
  uint8_t frequencyCounter; // how far in we are to frequency
  //CircularBuffer<uint32_t> circularBuffer((uint32_t*)NULL, 10);
  CircularBuffer<uint16_t> buffer;
  void (*spikeObserver)(Profiler&);
  bool spikeObserverLatch = false;

  uint16_t max, min;

  uint16_t _maxs[5];
  CircularBuffer<uint16_t> maxs;

public:
  Profiler()
  {
    // FIX: kludgey, don't know/remember how to initialize C++ instances properly
    // in this context
    buffer.setBuffer(intervals, profileCount);
    maxs.setBuffer(_maxs, 5);
    frequencyCounter = 0;
    frequency = 200;
    ResetMinMax();
  }
  void Profile();

  void ResetMinMax()
  {
    max = 0;
    min = -1;
    maxs.reset();
  }

  void setSpikeObserver(void (*spikeObserver)(Profiler&))
  {
    this->spikeObserver = spikeObserver;
  }

  uint16_t getAverage();

  uint32_t getIterator()
  {
    uint32_t value = overflowed ? 0xFFFF : iterator;
    overflowed = false;
    iterator = 0;
    return value;
  }

  CircularBuffer<uint16_t>& getMaxs() { return maxs; }

  uint16_t getMin() { return min; }
  uint16_t getMax() { return max; }

  void debugPrint() { buffer.debugPrint(); }
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

}

namespace Mal_Lib = FactUtilEmbedded;
namespace util = FactUtilEmbedded;

#endif