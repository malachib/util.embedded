#include "catch.hpp"

#include <fact/Menu.h>
#include <Console.h>

using namespace util;


void generic(IMenu::Parameters p)
{

}

MenuGeneric menuGeneric1(generic);
MenuGeneric menuGeneric2(generic);

class DummyMenu : public Menu
{
public:
  DummyMenu()
  {
    INFO("Constructing");
    add(menuGeneric1, F("generic"), F("Generic stuff"));
    add(menuGeneric2, F("generic2"), F("Generic stuff2"));
    INFO("Constructed");
  }

  void test1()
  {
    Parameters p(NULL, 0, NULL);
    INFO("Showing help");
    showHelp(p);
    INFO("SHowed help");
  }
};

SCENARIO( "Menu tests", "[menu]" )
{
  DummyMenu menu;

  std::string s = (char*) menuGeneric1.getName();

  REQUIRE(s == "generic");

  s = (char*) menuGeneric2.getName();

  REQUIRE(s == "generic2");

  menu.test1();
}
