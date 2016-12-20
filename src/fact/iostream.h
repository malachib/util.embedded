// Compatibility shim for targets (which seem to be many) who don't have an iostream
// implementation.  Also can and should serve as a wrapper class around Stream implementations
// found in libs like Arduino & mbed OS

extern "C"
{
  #include <stdint.h>
  #include <stdlib.h>
}

namespace FactUtilEmbedded { namespace std
{

typedef uint16_t streamoff;
typedef uint16_t streamsize;

} }

#ifdef ESP_OPEN_RTOS
#include "streams/iostream_esp8266.h"
#else
#warning "Unknown architecture"
#endif

namespace FactUtilEmbedded { namespace std
{

extern ostream cout;
extern istream cin;
extern ostream& clog;
extern ostream& cerr;


} }
