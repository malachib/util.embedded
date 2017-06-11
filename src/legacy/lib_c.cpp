#include "fact/lib.h"

namespace FactUtilEmbedded
{

bool stringBufferOutput(byte input,void*)
{
  if(input == 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

}
