#include <Arduino.h>
#include <fact/interrupt.h>

using namespace FactUtilEmbedded;

void setup()
{
  auto ctx = Interrupt.getContext(7);
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
