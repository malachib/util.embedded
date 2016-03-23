#include <Arduino.h>
#include "fact/Menu.h"

namespace FactUtilEmbedded
{
  void renderParameterError(uint8_t expectedParameterCount, uint8_t count)
  {
    cout.println();
    cout << F("Expected ") << expectedParameterCount;
    cout << F(" parameters but instead got ") << count;
  }
}
