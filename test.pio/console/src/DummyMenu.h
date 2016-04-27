#pragma once

#include <fact/Menu.h>
#include <fact/lib.h>

using namespace util;

class DummyMenu : public Menu
{
public:
  DummyMenu();

  void testHelp();
};

void dummyHandler(IMenu::Parameters p);

extern DummyMenu menu;
