#include "catch.hpp"

#include <fact/Menu.h>
#include <Console.h>

using namespace util;

class DummyMenu : public MenuHandler
{
public:
  void test1()
  {
    Parameters p(NULL, 0, NULL);
    showHelp(p);
  }
};

SCENARIO( "Menu tests", "[menu]" )
{
  DummyMenu menu;

  menu.test1();
}
