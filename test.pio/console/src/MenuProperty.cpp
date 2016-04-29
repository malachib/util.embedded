#include "features.h"
#include <fact/PropertyMenuCommand.h>
#include <fact/lib.h>
#include "DummyMenu.h"

using namespace util;

#ifdef FEATURE_MENUPROPERTY
const char PROP_VAL1_NAME[] PROGMEM = "val1";
int val1 = 3;
menu::Property<int> prop1(PROP_VAL1_NAME, &val1);
//menu::Property<int, layer1::Property> prop1(PROP_VAL1_NAME, 3);
#if FEATURE_MENUPROPERTY >= 1
menu::PropertyBase* _properties[] = { &prop1 };
#if FEATURE_MENUPROPERTY >= 2
//layer3::Array<menu::PropertyBase*> properties(_properties, 1);
#if FEATURE_MENUPROPERTY >= 3
SetPropertyMenuCommand setPropertyMenuCommand(_properties, 1);
GetPropertyMenuCommand getPropertyMenuCommand(_properties, 1);

//#include <initializer_list>

void setup_menuProperty()
{
  //std::initializer_list<int>* test;

  ::menu.add(getPropertyMenuCommand);
  ::menu.add(setPropertyMenuCommand);
}
#endif // 3
#endif // 2
#endif // 1
#endif // overall
