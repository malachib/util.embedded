#pragma once

struct FakeFloat
{
  bool negative = false;
  uint16_t integer = 0;
  uint32_t decimal = 0;

  // prec = how many decimal places out "decimal" value of 1 is
  // or, in other words, deepest precision.  Maximum allowed is 9
  void parse(char* source, uint8_t prec, char delimiter = 0);

  template <class T>
  static bool parseCharacter(char c, T* value, bool firstCharacter)
  {
    if(c >= '0' && c <= '9')
    {
      if(!firstCharacter) *value *= 10;

      *value += (c - '0');
      return true;
    }

    return false;
  }

  void subtract(FakeFloat& subtractor);
};
