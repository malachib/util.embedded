#include "Initializer.h"

bool Initializer::Initialize(const __FlashStringHelper*  (*initFunc)())
{
  state = Initializing;
  errorMessage = initFunc();
  if(errorMessage)
    state = Error;
  else
    state = Initialized;
}
