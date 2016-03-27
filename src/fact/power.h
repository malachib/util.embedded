#pragma once

#ifdef __AVR__
#include "hardware/power_avr.h"
#elif defined(ESP8266)
#include "hardware/power_esp8266.h"
#elif SAM_SERIES
#include "hardware/power_sam.h"
#else
#warning Unsupported architecture for power control
#endif
