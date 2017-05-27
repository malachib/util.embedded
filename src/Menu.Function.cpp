#include "fact/lib.h"
#include "fact/Menu.h"

// KLUDGE: mbed and Arduino have a collision on F macro and streambuf.h
// when using mbed defines it as its own thing, so redefine it here
#if !defined(ARDUINO)
#undef F
#include "fact/noduino_F.h"
#endif

namespace FactUtilEmbedded
{
    using namespace std;

  void renderParameterError(uint8_t expectedParameterCount, uint8_t count)
  {
    cout << endl;
    cout << F("Expected ") << (uint16_t)expectedParameterCount;
    cout << F(" parameters but instead got ") << (uint16_t)count;
  }
}
