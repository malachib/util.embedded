#ifdef __AVR__
#include "hardware/watchdog_avr.h"
#elif SAMD_SERIES
#include "hardware/watchdog_sam.h"
#else
#error Unsupported MCU
#endif
