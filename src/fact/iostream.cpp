// FIX: Needs attention, this will break non-shim POSIX compilation
#if defined(FEATURE_IOSTREAM_SHIM) || defined(__POSIX__)
#ifdef ESP_OPEN_RTOS
extern "C"
{
    #include "espressif/esp_common.h"
    #include "FreeRTOS.h"

    #include <unistd.h>
    #include <esp8266.h>
    #include <esp/uart.h>
    #include <stdio.h>
}
#else
#endif

#include "iostream.h"

namespace FactUtilEmbedded { namespace std
{

ostream cout;
istream cin;
ostream& clog = cout;
ostream& cerr = cout;

} }

#endif
