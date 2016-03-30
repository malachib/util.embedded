#include <Arduino.h>
#include <fact/interrupt.h>

// Including this only because shared platformini file expects it
// however, linker optimizes it back out as long as we don't really use it
#include <SoftwareSerial.h>

using namespace FactUtilEmbedded;

//#define PIN 10;

void setup()
{
#ifndef __AVR_ATtiny85__
  auto ctx = Interrupt.getContext(10);
#else
  auto ctx = Interrupt.getContext(PB0);
#endif
  //ctx.write();

  InterruptControl::PinChangeContextMulti multi;

  multi._or(ctx);
  multi.write();

  //PCICR = 1; // for 32u4, turn on only pin-change bank
  // turn on PC int vector
  //PCMSK0 = 1 << (uint8_t)6;
}

void loop()
{

}
