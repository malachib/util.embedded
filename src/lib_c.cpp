#include "lib.h"

//#include <avr/io.h>

namespace Mal_Lib
{
  /*
bool serialOutput(byte input,void*)
{
  if(input == 0)
  {
    Serial.println();
    return false;  
  }
  else
  {
    Serial.print((char)input);
    return true;
  }
}
*/
/*
 * Use memcpy_P instead
void pgm_extract_bytes(byte* input, byte* output, int length)
{
  while(length-- > 0)
  {
    *output = pgm_read_byte_near(input++);
    output++;
  }
}
*/

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

/*
 * Use strcpy_P instead
void pgm_extract_string(const char* input, char* output)
{
   PgmStringExtract pse;
   pse.Extract(input, output);
}
 */


void pgm_extract_bytes(byte* input, bool (*foo)(byte,void*), void* meta = NULL)
{
  byte result;
  
  do
  {
    result = pgm_read_byte_near(input++);
  } while(foo(result, meta));
}


void pgm_extract_bytes(byte* input, Stream* output, bool (*shouldContinue)(byte,void*), void* meta = NULL)
{
  byte result;
  
  do
  {
    result = pgm_read_byte_near(input++);
    output->print((char)result);
  } while(shouldContinue(result, meta));
}

#if AVR
void PgmStream::println(const char* input)
{
  pgm_extract_bytes((byte*)input, &stream, stringBufferOutput);
}
#endif
}
