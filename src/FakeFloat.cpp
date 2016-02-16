#include <Arduino.h>
#include "FakeFloat.h"

void FakeFloat::parse(char* source, uint8_t prec, char delimiter)
{
  bool integerMode = true;
  bool firstCharacter = true;
  bool negative = false;

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
          if(negative) integer *= -1;

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
