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
