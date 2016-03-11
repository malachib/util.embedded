template<class T> T fromString(const char* input);

template<> int fromString(const char* input)
{
  return atoi(input);
}

template<> float fromString(const char* input)
{
  return atof(input);
}

template<> const char* fromString(const char* input)
{
  return input;
}
