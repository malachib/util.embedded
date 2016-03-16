// standin lib redirecting arduino-style Serial to stdin/stdout

#include "vspde.h"
#include <iostream>

using namespace std;

IOStream Serial;

int IOStream::read()
{
  return cin.get();
}

int IOStream::available()
{
  return cin.tellg();
}
