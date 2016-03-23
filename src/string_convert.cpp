#include <Arduino.h>
#include "fact/string_convert.h"

const char VALIDATE_NULLSTR_ERROR[] PROGMEM = "Null String";
const char VALIDATE_STRTOOLONG_ERROR[] PROGMEM = "String too long";

const char TYPENAME_INT[] PROGMEM = "Integer";
const char TYPENAME_CHARPTR[] PROGMEM = "String";

// FIX: Unknown if these can safely live here.  
//  If I don't put them here and leave them in includes, I get linker dup errors
//  If I do put them in here, then I get linker ref not found errors
// For the time being disabling the feature (disabling validateInvokeNative)

template<> PGM_P validateString<const char*>(const char* input)
{
  if(input == NULL) return VALIDATE_NULLSTR_ERROR;
  return NULL;
}

template<> PGM_P validateString<char>(const char* input)
{
  if(strlen(input) > 1) return VALIDATE_STRTOOLONG_ERROR;
  // TODO: ensure input can be converted to char
  return NULL;
}


template<> PGM_P validateString<int>(const char* input)
{
  // TODO: ensure input can be converted to int
  return NULL;
}

template<> PGM_P validateString<float>(const char* input)
{
  // TODO: ensure input can be converted to float
  return NULL;
}



template<> char* toString(char* output, char input)
{
  output[0] = input;
  output[1] = 0;
  return output;
}


#ifdef __AVR__
#include <stdlib.h>

template<> char* toString(char* output, int input)
{
  return itoa(input, output, 10);
}


template<> char* toString(char* output, unsigned int input)
{
  return utoa(input, output, 10);
}
#else
template<> char* toString(char* output, int input)
{
  sprintf(output, "%d", input);
  return output;
}

#endif


template<> PGM_P getTypeName<const char*>()
{
  return TYPENAME_CHARPTR;
}

template<> PGM_P getTypeName<int>()
{
  return TYPENAME_INT;
}
