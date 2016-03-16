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
  //return cin.tellg();
  //return cin.rdbuf()->in_avail();
  //return (cin.peek() & ios_base::io_state::eofbit) ? 1 : 0;
  return cin.peek() == EOF ? 0 : 1;
}
