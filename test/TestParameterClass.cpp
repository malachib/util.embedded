#include "catch.hpp"

#include <fact/ipc.h>
#include <menu/Properties.h>

using namespace FactUtilEmbedded;

class TestParameterClass
{
  int value = 1;

public:
  void test()
  {
    value++;
  }


  void test2(int value)
  {
    this->value = value;
  }

  int getValue() { return value; }
};


static float ipc_test_value = 0;
std::string ipc_test_str = "";

void test1_ipc()
{
  ipc_test_value++;
}

void test2_ipc(float f)
{
  ipc_test_value = f;
}

void test3_ipc(float f, const char* str)
{
  ipc_test_value = f;
  ipc_test_str = str;
}

SCENARIO( "Low level parameter class tests", "[parameter-class]" )
{
  WHEN("Doing 0 parameter")
  {
    IPCHelper::create(test1_ipc).invoke();
    REQUIRE(ipc_test_value == 1);
  }
  WHEN("Doing 1 parameter")
  {
    auto m = IPCHelper::create(test2_ipc, 32.0F);
    m.invoke();
    REQUIRE(ipc_test_value == 32);
  }
  WHEN("Doing 2 parameter")
  {
    auto m2 = IPCHelper::create(test3_ipc, 7.0F, "lucky #7");
    m2.invoke();
    REQUIRE(ipc_test_value == 7);
    REQUIRE(ipc_test_str == "lucky #7");
  }
  WHEN("Doing 0 parameter, instance method")
  {
    TestParameterClass tpc;

    auto m = IPCHelper::create(&TestParameterClass::test, &tpc);
    m.invoke();

    REQUIRE(tpc.getValue() == 2);
  }
  WHEN("Doing 1 parameter, instance method")
  {
    TestParameterClass tpc;

    auto m = IPCHelper::create(&TestParameterClass::test2, &tpc, 7);

    IInvoker* invoker = &m;

    invoker->invoke();

    REQUIRE(tpc.getValue() == 7);
  }
  GIVEN("CallQueue class")
  {
    CallQueue<128, 4> callQueue;

    TestParameterClass tpc;
    auto m = IPCHelper::create(&TestParameterClass::test, &tpc);
    auto m2 = IPCHelper::create(&TestParameterClass::test2, &tpc, 7);

    INFO("Size of control struct m = " << sizeof(m));
    INFO("Size of control struct m2 = " << sizeof(m2));

    IInvoker* i = &m;

    WHEN("Putting calls into the queue")
    {
      callQueue.put(i);
      callQueue.put(&m2);
      //i->debugPrint();
    }

    WHEN("Retrieving calls from the queue")
    {
      INFO("Call #1");
      INFO("position_get = " << callQueue.queue.getPositionGet())
      IInvoker* invoker = callQueue.get();
      INFO("invoker = " << invoker);
      REQUIRE(memcmp(i, invoker, 32) == 0);
      REQUIRE(memcmp(&m, invoker, 32) == 0);
      //invoker->debugPrint();
      invoker->invoke();
      REQUIRE(tpc.getValue() == 2);

      INFO("Call #2");
      INFO("position_get = " << callQueue.queue.getPositionGet())
      IInvoker* invoker2 = callQueue.get();
      INFO("invoker2 = " << invoker2);
      //REQUIRE(memcmp(&m, &m, 8) == 0);
      //invoker2->debugPrint();
      //invoker2->invoke();
      REQUIRE(tpc.getValue() == 7);
    }
  }

  GIVEN("")
  {

  }

}
