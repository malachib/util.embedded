#include <iostream>
#include <ncurses.h>

#ifndef UNUSED
#include <Console.h>

#include <fact/MenuFunction.h>

using namespace util;

//char _helper[] = "HELPER";
//const __FlashStringHelper* helper = _helper;

/*
inline basic_ostream<char,
      std::__1::char_traits<char> >& operator <<(ostream& out, const __FlashStringHelper* arg) 
{ out << (const char*)(arg); return out; }
*/


class HELPER;
#define F2(string_literal) (reinterpret_cast<const HELPER *>(PSTR(string_literal)))

const HELPER* helper2 = F2("Hi");

inline ostream& operator <<(ostream& out,  const HELPER* arg) 
{ out << (const char*)(arg); return out; }

/*
inline ::std::ostream& operator <<(::std::ostream& out,  const __FlashStringHelper* arg) 
{ out << (const char*)(arg); return out; }
*/

void test(IMenu::Parameters p)
{
    cout << "Hi helper: "  << helper2;
  cout << "Hi from test!" << endl;
}

void test2(const char* input)
{
  cout << "I got: " << input << endl;
}

void anotherTest(const char* input)
{
  cout << "I really got: " << input << endl;
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
ConsoleMenu console(&mainMenu);

int main()
{
    cout << "Hi" << endl;
    
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
#else

using namespace std;

int main()
{
    cout << "Hello world" << std::endl;
    endwin();
}
#endif
