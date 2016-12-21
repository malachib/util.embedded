#include "fact/lib.h"
#include "fact/Menu.h"


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
