#include <Arduino.h>
#include "fact/gc.h"

namespace FactUtilEmbedded
{
  _gc_ptr gc_manager::alloc(uint16_t size)
  {
    _gc_ptr* head = getHead();
  }
}
