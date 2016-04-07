#include "catch.hpp"

#include <fact/ipc.h>
#include <menu/Properties.h>


void test1_ipc()
{
  printf("\r\nHello world");
}


SCENARIO( "Low level parameter class tests", "[parameter-class]" )
{
  createIPCMessage(&test1_ipc);
  GIVEN("")
  {

  }

}
