//#include <Arduino.h>
#include "fact/c_types.h"
#include "fact/string_convert.h"
#include <ctype.h>

const char VALIDATE_NULLSTR_ERROR[] PROGMEM = "Null String";
const char VALIDATE_STRTOOLONG_ERROR[] PROGMEM = "String too long";
const char VALIDATE_FORMAT_ERROR[] PROGMEM = "Invalid format";

const char TYPENAME_INT[] PROGMEM = "Integer";
const char TYPENAME_CHARPTR[] PROGMEM = "String";

// FIX: Unknown if these can safely live here.
//  conditions occur while compiling for ESP8266
//  If I don't put them here and leave them in includes, I get linker dup errors
//  If I do put them in here, then I get linker ref not found errors
// For the time being disabling the feature (disabling validateInvokeNative)

// TODO: combine validate/convert functions since they happen lock step, this way
//   we can avoid double-conversion at times

template<> PGM_P validateString<char>(const char* input)
{
  DO_VALIDATE_FORNULL;
  if(strlen(input) > 1) return VALIDATE_STRTOOLONG_ERROR;

  return nullptr;
}


template<> PGM_P validateString<int>(const char* input)
{
  DO_VALIDATE_FORNULL;
  do
  {
    if(!isdigit(*input)) return VALIDATE_FORMAT_ERROR;
  } while(*++input);

  return nullptr;
}


#ifndef __AVR__
template<> PGM_P validateString<float>(const char* input)
{
  DO_VALIDATE_FORNULL;
  char* str_end;
  strtof(input, &str_end);

  return input == str_end ? VALIDATE_FORMAT_ERROR : nullptr;
}
#endif


template<> PGM_P validateString<double>(const char* input)
{
  DO_VALIDATE_FORNULL;
  char* str_end;
  strtod(input, &str_end);

  return input == str_end ? VALIDATE_FORMAT_ERROR : nullptr;
}


template <> PGM_P validateString<unsigned short>(const char* input)
{
    DO_VALIDATE_FORNULL;
    char* str_end;

    strtoul(input, &str_end, 10);

    return input == str_end ? VALIDATE_FORMAT_ERROR : nullptr;
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

template<> char* toString(char* output, uint8_t input)
{
  sprintf(output, "%u", input);
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
