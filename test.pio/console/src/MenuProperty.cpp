#include "features.h"
#include <fact/PropertyMenuCommand.h>
#include <fact/lib.h>
#include "DummyMenu.h"

using namespace util;

#ifdef FEATURE_MENUPROPERTY
const char PROP_VAL1_NAME[] PROGMEM = "val1";
int val1 = 3;
menu::Property<int> prop1(PROP_VAL1_NAME, &val1);
#if FEATURE_MENUPROPERTY >= 1
menu::PropertyBase* _properties[] = { &prop1 };
#if FEATURE_MENUPROPERTY >= 2
layer3::Array<menu::PropertyBase*> properties(_properties, 1);
#if FEATURE_MENUPROPERTY >= 3
SetPropertyMenuCommand setPropertyMenuCommand(properties);
GetPropertyMenuCommand getPropertyMenuCommand(properties);

void setup_menuProperty()
{
  ::menu.add(getPropertyMenuCommand);
  ::menu.add(setPropertyMenuCommand);
}
#endif // 3
#endif // 2
#endif // 1
#endif // overall
