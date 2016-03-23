#include "catch.hpp"

#include <Arduino.h>
#include <fact/MenuFunction.h>

using namespace util;

int testFunc1(int value)
{
  REQUIRE(value == 123);
  //INFO("Got here with value: ")
  //INFO(value);
  return -1;
}


int testFunc2(int value, float value2)
{
  REQUIRE(value == 123);
  REQUIRE(value2 == 7.7f);
  //INFO("Got here with value: ")
  //INFO(value);
  return -1;
}

/*
class DummyMenuFunction : public MenuFunction<int, int>
{
public:
  DummyMenuFunction(_handler handler) : MenuFunction<int, int>(handler) {}

  void test1()
  {
    const char* _p[] = { "123" };
    Parameters p((char**)_p, 1, NULL);

    handleCommand(p);
  }
};
*/
const char NAME[] = "testFunc2";
const char DESC[] = "a test func 2";

auto mf2 = createMenuFunction(testFunc2, NAME, DESC);
CREATE_MENUFUNCTION(mf3, testFunc1, "a test func");

class MenuFunctionMenu : public Menu
{
public:
  MenuFunctionMenu()
  {
    add(mf2);
    add(mf3);
  }

  void test1()
  {
    auto head = getHeadMenu();

    REQUIRE(head == &mf2);
    REQUIRE(head->getNext() == &mf3);
    REQUIRE(head->getNext()->getNext() == NULL);

    auto _head = (MenuBase*) head;

    REQUIRE(_head == &mf2);
    REQUIRE(((MenuBase*)_head->getNext()) == &mf3);
  }
};

//Console console;

SCENARIO( "MenuFunction tests", "[menu-function]" )
{
  //MenuFunction<int, int> menuFunction1(testFunc1);
  //DummyMenuFunction menuFunction1(testFunc1);

  GIVEN("A string to integer conversion")
  {
    auto value = fromString<int>("123");

    REQUIRE(value == 123);
  }
  GIVEN("Low level invoker test")
  {
    const char* _p[] = { "123" };
    IMenu::Parameters p(_p, 1, NULL);
    invoke(testFunc1, p);
  }
  GIVEN("Low level invoker test 2")
  {
    const char* _p[] = { "123", "7.7" };
    IMenu::Parameters p(_p, 2, NULL);
    invoke(testFunc2, p);
  }
  GIVEN("The simplest case MenuFunction int, int")
  {
    //menuFunction1.test1();
  }
  GIVEN("Enhanced MenuFunction2 class")
  {
    auto mf = createMenuFunction(testFunc2, NAME, DESC);
    //MenuFunction2 mf2(testFunc2);
    //MFWrapper<int> mfw(testFunc2);

    const char* _p[] = { "123", "7.7" };
    IMenu::Parameters p(_p, 2, NULL);
    mf._handleCommand(p);
    mf2._handleCommand(p);
    //_handleCommand;
  }
  GIVEN("Holding MenuFunctions in a Menu")
  {
    MenuFunctionMenu menu;

    menu.test1();
  }
}
