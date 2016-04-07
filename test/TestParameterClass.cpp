#include "catch.hpp"

#include <fact/ipc.h>
#include <menu/Properties.h>


void test1_ipc()
{
  printf("\r\nHello world");
}

void test2_ipc(float f)
{
  printf("\r\nHello world: %f", f);
}

SCENARIO( "Low level parameter class tests", "[parameter-class]" )
{
  createIPCMessage(&test1_ipc);
  auto m = createIPCMessage(&test2_ipc);
  m.invoke();
  GIVEN("")
  {

  }

}
