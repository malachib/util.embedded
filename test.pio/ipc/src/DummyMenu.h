#pragma once


class DummyMenu : public Menu
{
public:
  DummyMenu();

  void testHelp();
};

void dummyHandler(IMenu::Parameters p);
void testInvoker();
