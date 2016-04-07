#include "catch.hpp"

#include <fact/ipc.h>
#include <menu/Properties.h>


class TestParameterClass
{
  int value = 1;

public:
  void test()
  {
    printf("\r\nFrom parameter class tester: %d", value);
  }


  void test2(int value)
  {
    this->value = value;

    printf("\r\nFrom parameter class tester: %d", value);
  }
};

void test1_ipc()
{
  printf("\r\nHello world");
}

void test2_ipc(float f)
{
  printf("\r\nHello world: %f", f);
}

void test3_ipc(float f, const char* str)
{
  printf("\r\nHello world: %f:%s", f, str);
  printf("\r\n");
}

SCENARIO( "Low level parameter class tests", "[parameter-class]" )
{
  WHEN("Doing 0 parameter")
  {
    //createIPCMessage(test1_ipc).invoke();
    IPCHelper::create(test1_ipc).invoke();
  }
  WHEN("Doing 1 parameter")
  {
    auto m = IPCHelper::create(test2_ipc, 32.0F);
    m.invoke();
  }
  WHEN("Doing 2 parameter")
  {
    auto m2 = IPCHelper::create(test3_ipc, 7.0F, "lucky #7");
    m2.invoke();
  }
  WHEN("Doing 0 parameter, instance method")
  {
    TestParameterClass tpc;

    auto m = IPCHelper::create(&TestParameterClass::test, &tpc);
    m.invoke();
  }
  WHEN("Doing 1 parameter, instance method")
  {
    TestParameterClass tpc;

    auto m = IPCHelper::create(&TestParameterClass::test2, &tpc, 7);

    IInvoker* invoker = &m;

    invoker->invoke();
  }

  GIVEN("")
  {

  }

}
