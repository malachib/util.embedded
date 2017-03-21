#include "hal.h"
#ifdef ARDUINO
#include <Arduino.h>
#else
#include "../fact/noduino.h"
#endif

namespace FactUtilEmbedded {

Led::Led(uint8_t pinNumber, bool autoInitOutput) : DigitalPin(pinNumber)
{
    // I'd expect LEDs 99% of the time want to be in OUTPUT mode
	// TODO: notate... again not sure why i need to exclude PSoC here...
	// could be cuz of global init conflicts
#if !CYPRESS_PSOC && !WIN32
    if(autoInitOutput)
      SetMode(OUTPUT);
#endif
}

Led::Led() : Led(13, true)
{

}
    
}
