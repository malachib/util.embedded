#pragma once

template<class T> T fromString(const char* input);

template<> inline unsigned char fromString(const char* input)
{
  return (unsigned char) atoi(input);
}

template<> inline int fromString(const char* input)
{
  return atoi(input);
}

template<> inline float fromString(const char* input)
{
  return atof(input);
}

template<> inline const char* fromString(const char* input)
{
  return input;
}
