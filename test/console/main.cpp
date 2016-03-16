#include <iostream>
#include <Console.h>

using namespace util;

void test(IMenu::Parameters p)
{
  
}


MenuGeneric menuGeneric(test);

class MainMenu : public Menu
{
public:
  MainMenu()
  {
    add(menuGeneric, F("test"), F("test entry"));
  }  
};

MainMenu mainMenu;
ConsoleMenu console(&mainMenu);

int main()
{
  for(;;)
  {
    if(Serial.available())
    {
      Serial << "character available: " << (char)Serial.read();
    }
    console.handler();
  }
}
