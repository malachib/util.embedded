#include <Arduino.h>
#include "FakeFloat.h"

void FakeFloat::parse(char* source, uint8_t prec, char delimiter)
{
  bool integerMode = true;
  bool firstCharacter = true;

  for(char c = *source; c != delimiter; c = *++source)
  {
    //cout << F("Current character: ") << c;
    //cout.println();

    if(integerMode)
    {
      if(!parseCharacter(c, &integer, firstCharacter))
      {
        if(c == '-' && firstCharacter)
        {
          negative = true;
        }
        else if(c == '.')
        {
          integerMode = false;
          firstCharacter = true;
          // so that we don't assign firstCharacter to false
          continue;
        }
      }

      firstCharacter = false;
    }
    else
    {
      //cout << F("Current value: ") << decimal;
      //cout.println();

      if(prec--)
      {
        parseCharacter(c, &decimal, firstCharacter);
        firstCharacter = false;
      }
      else
      {
        // that's all we allow is prec # of places, and it has been exceeded
        // so return
        return;
      }
    }
  }

  while(prec--)
    decimal *= 10;
}


// might be ready for prime time
void FakeFloat::subtract(FakeFloat& subtractor)
{
  integer -= subtractor.integer;

  // if subtractor.decimal > decimal, i.e.:
  // 1.099 - 1.100
  //   firstly, integer by now will be 0 instead of 1, so we'll be at
  //   0.099. [subtractor.decimal - decimal] = .01
  //   (other way around would be -.01 which doesn't play nice with unsigned)
  //   = -0.001
  // 2.099 - 1.100
  //   = 0.999
  // 1.095 - 1.100
  //   = -0.005
  // 2.095 - 3
  //   = -0.905
  // 2.095 - 3.090
  // otherwise, standard:
  // 1.100 - 1.099
  //  = 1.01  [decimal - subtractor.decimal] = .01
  if(subtractor.decimal > decimal)
  {
    if(integer == 0 && !negative)
      negative = true;
    else
      integer--;

    decimal = subtractor.decimal - decimal;
  }
  else
    decimal -= subtractor.decimal;
}
