#include "Math.h"


class FloatConversion
{
public:
  double value;
  double divisor;
  bool foundNonZero = false;
  void* context = context;
  outputCharFunc outputChar;

  void dtostrf_helper();
};

void FloatConversion::dtostrf_helper()
{
  double divided = value / divisor;
  // fast-floor and dump decimal --- hopefully
  auto floored = (uint8_t)divided;
  char digit = '0' + floored;

  if(!foundNonZero && floored > 0) foundNonZero = true;

  if(foundNonZero)  outputChar(context, digit);

  double subtractor = floored * divisor;

  value -= subtractor;

  divisor /= 10;
}


void dtostrf_funcnew(double value, uint8_t prec, outputCharFunc outputChar, void* context, bool outputDelimiter)
{
  //double divisor = 10000000000;
}

void dtostrf_func(double value, uint8_t prec, outputCharFunc outputChar, void* context, bool outputDelimiter)
{
  FloatConversion fc;

  fc.value = value;
  fc.outputChar = outputChar;
  fc.context = context;
  fc.divisor = 100000000000;

  if(value < 0) outputChar(context, '-');

  while(fc.divisor > 0.1)
    fc.dtostrf_helper();

  if(prec > 0)
  {
    outputChar(context, '.');

    while(prec-- > 0)
      fc.dtostrf_helper();
  }

  if(outputDelimiter)
    outputChar(context, 0);
}
