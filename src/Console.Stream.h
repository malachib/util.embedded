#pragma once

#ifdef CONSOLE_STREAM
#define cout CONSOLE_STREAM
#define cin CONSOLE_STREAM
#else
// temporary until we fully enable this inside Console class itself
#ifdef SAML_SERIES
#define cout SerialUSB
#define cin SerialUSB
#elif not defined(__AVR_ATtiny85__)
#define cout Serial
#define cin Serial
#endif
#endif

#ifdef CONSOLE_STREAM_EXTERN
// FIX: pretty kludgey
#if CONSOLE_STREAM_EXTERN == SoftwareSerial
#include <SoftwareSerial.h>
#endif
extern CONSOLE_STREAM_EXTERN cout;
#endif

// NOTE: kinda kludgey, this discovers if no cout exists
// and if so makes a dummy one so that at least the menu
// stuff will compile (and be optimized out)
#if !defined(cout) and !defined(CONSOLE_STREAM_EXTERN)
class DummyStream : public Print
{
public:
  virtual size_t write(uint8_t) override { return 1; }
  uint8_t available() { return 0; }
  char read() { return ' '; }
} _dummyStream;

#define cin _dummyStream
#define cout _dummyStream
#endif
