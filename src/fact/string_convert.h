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

extern const char VALIDATE_NULLSTR_ERROR[];
extern const char VALIDATE_STRTOOLONG_ERROR[];
extern const char VALIDATE_FORMAT_ERROR[];

extern const char TYPENAME_INT[];
extern const char TYPENAME_CHARPTR[];

template<class T> PGM_P validateString(const char* input);
template<class T> char* toString(char* output, T input);
template<class T> PGM_P getTypeName();

#ifdef VALIDATE_FEATURE_NULLCHECK
#define DO_VALIDATE_FORNULL { if(!input) return VALIDATE_NULLSTR_ERROR; }
#else
#define DO_VALIDATE_FORNULL
#endif

template<> inline PGM_P validateString<const char*>(const char* input)
{
  DO_VALIDATE_FORNULL;
  return nullptr;
}
