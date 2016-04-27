#include "catch.hpp"

#include <fact/PropertyMenuCommand.h>
#include <fact/lib.h>

using namespace util;

SCENARIO("Property Menu Command test", "[property-menu-command]")
{
  //GIVEN("layer 2")
  {
    int value = 3;
    const char* parameters[] = { "get" };
    const char* parameters2[] = { "set", "5" };

    PropertyMenuCommand<int> val(&value);

    IMenu::Parameters p(parameters, 1, nullptr);

    val._handleCommand(p);

    IMenu::Parameters p2(parameters2, 2, nullptr);

    val._handleCommand(p2);

    REQUIRE(value == 5);
  }
}
