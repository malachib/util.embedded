#include <Arduino.h>

#ifdef UNUSED
#if SAM_SERIES
void WDT_Handler(void)
{
  // placeholder: Do we need to do something like this for WDT like RTC needs?
  // RTC->MODE2.INTFLAG.reg = RTC_MODE2_INTFLAG_ALARM0; // must clear flag at end
}
#endif
#endif
