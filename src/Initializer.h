#include <Arduino.h>

class Initializer
{
  enum State : uint8_t
  {
    Uninitialized = 0,
    Initializing = 1,
    Initialized = 2,
    Error = 3
  };

  State state;
  const __FlashStringHelper* errorMessage;

  bool Initialize(const __FlashStringHelper* (*initFunc)());
};
