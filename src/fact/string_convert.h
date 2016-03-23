#pragma once

template<class T> T fromString(const char* input);

template<> inline unsigned char fromString(const char* input)
{
  // TODO: use strtol and friends if available, atoi is deprecated
  return (unsigned char) atoi(input);
}

template<> inline int fromString(const char* input)
{
  // TODO: use strtol and friends if available, atoi is deprecated
  return atoi(input);
}

template<> inline float fromString(const char* input)
{
  return atof(input);
}

template<> inline unsigned long fromString(const char* input)
{
  return strtoul(input, NULL, 10);
}

template<> inline const char* fromString(const char* input)
{
  return input;
}

template<> inline char fromString(const char* input)
{
  return input[0];
}

template<class T> PGM_P validateString(const char* input);

template<> PGM_P validateString<const char*>(const char* input)
{
  if(input == NULL) return F("Null string");
  return NULL;
}

template<> PGM_P validateString<char>(const char* input)
{
  if(strlen(input) > 1) return F("String too long");
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

template<class T> char* toString(char* output, T input);

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

template<class T> PGM_P getTypeName();

template<> PGM_P getTypeName<const char*>()
{
  return F("String");
}

template<> PGM_P getTypeName<int>()
{
  return F("Integer");
}
