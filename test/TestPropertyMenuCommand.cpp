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
    
    PropertyMenu::Item<int> item("item1", &value);
    layer5::Property<int> prop2(&value);
    PropertyMenu::ItemRef<int> item2("item2", prop2);
    PropertyMenu::ItemBase* _items[] = { &item, &item2 };
    layer3::Array<PropertyMenu::ItemBase*> items(_items, 2);
    
    GetPropertyMenuCommand getter(items);
    SetPropertyMenuCommand setter(items);

    const char* parameters_set[] = { "set", "item2", "8" };
    
    IMenu::Parameters p_set(parameters_set, 3, nullptr);
    
    setter._handleCommand(p_set.inc());
    
    const char* parameters3[] = { "get", "item1" };
    
    IMenu::Parameters p3(parameters3, 2, nullptr);

    printf("\r\nRetrieving %s: ", parameters3[1]);
    // we do inc because MenuHandler does this too (thus
    // the difficulty in combining many commands together)
    getter._handleCommand(p3.inc());
  }
}
