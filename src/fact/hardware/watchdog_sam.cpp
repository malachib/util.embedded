#include <Arduino.h>

//#ifdef UNUSED
#if SAMD_SERIES

int wdt_handler_called = 0;
extern "C" void WDT_Handler();

extern "C" void WDT_Handler(void)
{
  ++wdt_handler_called;
  // placeholder: Do we need to do something like this for WDT like RTC needs?
  // RTC->MODE2.INTFLAG.reg = RTC_MODE2_INTFLAG_ALARM0; // must clear flag at end
}
#endif
//#endif
