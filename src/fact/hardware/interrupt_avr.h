#include <avr/interrupt.h>

#ifdef __AVR_ATmega32U4__
#define AVR_PCICR_COUNT 1
#elif __AVR_ATmega328P__
#define AVR_PCICR_COUNT 3
#elif __AVR_ATtiny85__
#define AVR_PCICR_COUNT 1
#else
#error Unsupported MPU
#endif

// of general (but not specific) interest: http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__interrupts.html
namespace FactUtilEmbedded
{
  class InterruptControl
  {
  public:
    class PinChangeContext
    {
      const uint8_t _PCICR; // represents whether we're talking about PCICR0, 1, 2, etc.
      const uint8_t _PCMSK; // bit position to place into candidate PCMSK register

      // note, PCICR could hold multiple enables, but PCMSK can only hold one
    public:
      PinChangeContext(const uint8_t _PCICR, const uint8_t _PCMSK) :
        _PCICR(1 << _PCICR), _PCMSK(1 << _PCMSK) {}

      void write()
      {
        PCICR = _PCICR;

        if(_PCICR & 0b00000001)
        {
          PCMSK0 = _PCMSK;
        }
#if AVR_PCICR_COUNT > 1
        if(_PCICR & 0b00000010)
        {
          PCMSK1 = _PCMSK;
        }
#endif
#if AVR_PCICR_COUNT > 2
        if(_PCICR & 0b00000100)
        {
          PCMSK2 = _PCMSK;
        }
#endif
      }

      // FIX: kludge to get around difficulty of friend access between inner classes
      // although some would applaud extra getters
      uint8_t getPCICR() const { return _PCICR; }
      uint8_t getPCMSK() const { return _PCMSK; }
    };

    class PinChangeContextMulti
    {
      uint8_t _PCICR = 0;  // represents all enabled PCICR's
      uint8_t _PCMSK0 = 0; // bit position to place into candidate PCMSK register
#if AVR_PCICR_COUNT > 1
      uint8_t _PCMSK1 = 0; // bit position to place into candidate PCMSK register
#endif
#if AVR_PCICR_COUNT > 2
      uint8_t _PCMSK2 = 0; // bit position to place into candidate PCMSK register
#endif
      // once we determine AVR mega 328 can optimize this well, use the array version
      uint8_t _PCMSK[AVR_PCICR_COUNT];

    public:
      void _or(const PinChangeContext& c)
      {
        _PCICR |= c.getPCICR();

        if(c.getPCICR() & 0b00000001)
        {
          PCMSK0 |= c.getPCMSK();
        }
#if AVR_PCICR_COUNT > 1
        if(c.getPCICR() & 0b00000010)
        {
          PCMSK1 |= c.getPCMSK();
        }
#endif
#if AVR_PCICR_COUNT > 2
        if(c.getPCICR() & 0b00000100)
        {
          PCMSK2 |= c.getPCMSK();
        }
#endif
      }

      void write()
      {
        PCICR = _PCICR;

        if(_PCMSK0 != 0)
        {
          PCMSK0 = _PCMSK0;
        }
#if AVR_PCICR_COUNT > 1
        if(_PCMSK1 != 0)
        {
          PCMSK1 = _PCMSK1;
        }
#endif
#if AVR_PCICR_COUNT > 2
        if(_PCMSK2 != 0)
        {
          PCMSK2 = _PCMSK2;
        }
#endif
      }
    };

    // retrieve based on arduino pin mapping
    static PinChangeContext getContext(uint8_t pin)
    {
#ifdef __AVR_ATmega32U4__
      // building this based on what I see in pins_arduino for the 32u4
      switch(pin)
      {
        case  9: return PinChangeContext(0, 5);
        case 10: return PinChangeContext(0, 6);
        case 11: return PinChangeContext(0, 7);
        case 14: return PinChangeContext(0, 3); // MISO/PB3
        case 16: return PinChangeContext(0, 2); // MOSI/PB2
      }
#elif __AVR_ATtiny85__
#elif __AVR_ATmega328P__
#endif
    }
  };

  extern InterruptControl Interrupt;
}
