#include "rtc.h"
#include "pm.h"
#include "gclk.h"

#if SAMD_SERIES

namespace FactUtilEmbedded { namespace atmel { namespace sam
{
  void RealTimeClock::begin()
  {
    //PowerManager::RTC::enable();
    GenericClock::set_divisor<2>(4);
    GenericClock::enable_generator<2>(GCLK_GENCTRL_DIVSEL | GCLK_GENCTRL_SRC_XOSC32K);
    GenericClock::enable_clock<2>(RTC_GCLK_ID << GCLK_CLKCTRL_ID_Pos);
  }

}}}

#endif
