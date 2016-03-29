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
// Beware not to actually use Console/Menu stuff with this DummyStream
#if !defined(cout) and !defined(CONSOLE_STREAM_EXTERN)
class DummyStream : public Stream
{
public:
  virtual size_t write(uint8_t) override { return 1; }
  virtual int available() override { return 0; }
  virtual int read() override { return ' '; }
  virtual int peek() override { return -1; }
  virtual void flush() override {}

  void begin(uint32_t bps) {}
};

extern DummyStream _dummyStream;

#define cin _dummyStream
#define cout _dummyStream
#endif

#ifdef CONSOLE_FEATURE_COUT
#define MENU_DECLARE_COUT Stream& out = p.console->getOut();
#else
#define MENU_DECLARE_COUT Stream& out = cout;
#endif
