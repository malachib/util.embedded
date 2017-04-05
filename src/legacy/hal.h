#pragma once

#if !defined(CYPRESS_PSOC) && !defined(NODUINO)
#include <Arduino.h>
#endif

#include "../Handler.h"

namespace FactUtilEmbedded
{

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
#if !defined(CYPRESS_PSOC) && !defined(NODUINO)
    digitalWrite(pinNumber, signal);
#endif
  }
  void SetHigh() const
  {
#if !defined(CYPRESS_PSOC) && !defined(NODUINO)
    Write(HIGH);
#endif
  }
  void SetLow() const
  {
#if !defined(CYPRESS_PSOC) && !defined(NODUINO)
    Write(LOW);
#endif
  }
  inline void SetMode(uint8_t mode) const
  {
#if !defined(CYPRESS_PSOC) && !defined(NODUINO)
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

}
