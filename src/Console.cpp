#include "Console.h"

using namespace FactUtilEmbedded;


void Console::handler()
{
  while(cin.available() > 0)
  {
    char received = cin.read();

#ifdef CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR
    if(processInput(this, received))
    {

    }
    else
#endif
    if(received == '\n' || received == 13)
    {
      cout.println();

      int paramCounter = 0;
      char* parameters[10];

      inputLine[inputPos] = 0;

      // No input = just show prompt again
      if(inputPos == 0)
      {
        showPrompt();
        return;
      }

      //Serial << F("Submitting command: ") << inputLine << F("\n");

      parameters[0] = inputLine;

      for(int i = 0; i < inputPos; i++)
      {
        if(inputLine[i] == ' ')
        {
          inputLine[i] = 0;
          //Serial << F("param# ") << paramCounter << F(" = ") << parameters[paramCounter];
          paramCounter++;
          // FIX: preload code is kludgey, replace with something better (wastes one slot)
          // if we can sneak ahead one character safely, preload next paramCounter
          if(i + 1 < inputPos)
          {
            parameters[paramCounter] = &inputLine[i + 1];
          }
        }
      }

      handleCommand(Parameters(parameters, paramCounter + 1, this));

      cout.println();

      showPrompt();

      inputPos = 0;
    }
    else
    {
      cout << received;
      inputLine[inputPos++] = received;
    }
  }
}


void ConsoleMenuHandler::handleCommand(Parameters p)
{
  getActiveMenu()->handleCommand(p);
}



void ConsoleMenuHandler::showPrompt()
{
  for(int i = 0; i < breadCrumbPos; i++)
  {
    breadCrumb[i]->showPrompt();
    if(i != (breadCrumbPos - 1)) cout << ' ';
  }

  cout << F("> ");
}

#if defined(CONSOLE_FEATURE_AUTOCOMPLETE) && defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)
bool ConsoleMenuHandler::processInput(Console* console, char received)
{
  return getActiveMenu()->processInput(console, received);
}
#endif

void MenuEnumerator::add(Menu& menu)
{
  menus.add(&menu);
}

void MenuEnumerator::add(MenuGeneric& menu, const __FlashStringHelper* name, const __FlashStringHelper* description)
{
  menu.setDesc(name, description);
  menus.add(&menu);
}

Menu* MenuEnumerator::canHandle(IMenu::Parameters p)
{
  Menu* menu = (Menu*) menus.getHead();

  while(menu != NULL)
  {
    if(menu->canHandle(p))
      return menu;

    menu = (Menu*) menu->getNext();
  }

  return NULL;
}
