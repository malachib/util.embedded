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

#ifdef FEATURE_IOS_STREAMBUF_FULL
ostream cout;
istream cin;
#elif defined(__POSIX__) || defined(ESP_OPEN_RTOS)
ostream cout(*stdout);
istream cin(*stdin);
#else
#warning "Be sure to define your own explicit cout and cin, defaults not specified for this platform"
#endif
ostream& clog = cout;
ostream& cerr = cout;

} }
