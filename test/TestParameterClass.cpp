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

void test3_ipc(float f, char* str)
{
  printf("\r\nHello world: %f:%s", f, str);
  printf("\r\n");
}

SCENARIO( "Low level parameter class tests", "[parameter-class]" )
{
  WHEN("Doing 0 parameter")
  {
    createIPCMessage(&test1_ipc).invoke();
  }
  WHEN("Doing 1 parameter")
  {
    auto m = createIPCMessage(&test2_ipc);
    m.parameters.param1 = 32;
    m.invoke();
  }
  WHEN("Doing 2 parameter")
  {
    auto m2 = createIPCMessage(&test3_ipc);
    m2.parameters.param1 = 7;
    m2.parameters.param2 = "lucky #7";
    m2.invoke();
  }

  GIVEN("")
  {

  }

}
