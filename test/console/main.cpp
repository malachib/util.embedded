#include <iostream>
#include <Console.h>

#include <fact/MenuFunction.h>

using namespace util;

void test(IMenu::Parameters p)
{
  cout << "Hi from test!";
  cout.println();
}

void test2(const char* input)
{
  cout << "I got: " << input;
  cout.println();
}

void anotherTest(const char* input)
{
  cout << "I really got: " << input;
  cout.println();
}

MenuGeneric menuGeneric(test);
CREATE_MENUFUNCTION(menuTest2, test2, "test entry #2");
CREATE_MENUFUNCTION(menuAnotherTest, anotherTest, "test entry #3");

class MainMenu : public Menu
{
public:
  MainMenu()
  {
    add(menuGeneric, F("test"), F("test entry"));
    add(menuTest2);
    add(menuAnotherTest);
  }
};

MainMenu mainMenu;
ConsoleMenu console(&mainMenu, cout);

int main()
{
  for(;;)
  {
    /*
    if(Serial.available())
    {
      Serial << "character available: ";
      Serial << (char)Serial.read();
    }*/
    console.handler();
  }
}
