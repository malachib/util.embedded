#include <Arduino.h>
#include "Tokenizer.h"

bool Tokenizer::isDelimiter(char input)
{
  char* d = delimiters;

  while(*d)
  {
    if(input == *d)
      return true;

    d++;
  }

  return false;
}

bool Tokenizer::parse(char input)
{
  if(isDelimiter(input))
  {
    buffer[bufferPos] = 0;
    return true;
  }
  else
  {
    buffer[bufferPos++] = input;
    return false;
  }
}


bool TokenizerInPlace::parse()
{
  if(isDelimiter(buffer[bufferPos]))
    return true;

  bufferPos++;
  return false;
}


char* TokenizerInPlace::parseTokenDestructive()
{
  // loop through and find token
  parseToken();

  // destructively write to original buffer , cuz I know it's safe
  // in this context - we'll be overwriting the comma
  return getBufferDestructive();
}
