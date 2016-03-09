#include "Console.h"

using namespace FactUtilEmbedded;

// technically this is a stack opt
#ifdef MEMORY_OPT_DATA
#define CONSOLE_BEHAVIOR_MAX_PARAMETER 4
#else
#define CONSOLE_BEHAVIOR_MAX_PARAMETER 8
#endif


// TODO: refactor code so that handler is an inline call and processInput
// contains all the smarts (not inline call).  This way, we can enable/disable
// CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR feature from an #include rather than
// a -D switch
void Console::handler()
{
  while(cin.available() > 0)
  {
    char received = cin.read();

    if(processInput(this, received))
    {

    }
    else
    if(received == '\n' || received == 13)
    {
      cout.println();

      int paramCounter = 0;
      char* parameters[CONSOLE_BEHAVIOR_MAX_PARAMETER];

      inputLine[inputPos] = 0;

      // No input = just show prompt again
      if(inputPos == 0)
      {
        showPrompt();
        return;
      }

#ifdef DEBUG
      cout << F("Submitting command: ") << inputLine << F("\n");
#endif

      parameters[0] = inputLine;

      for(int i = 0; i < inputPos; i++)
      {
        if(inputLine[i] == ' ')
        {
          inputLine[i] = 0;
#ifdef DEBUG
          cout << F("param# ") << paramCounter << F(" = ") << parameters[paramCounter];
#endif
          paramCounter++;
          // FIX: preload code is kludgey, replace with something better (wastes one slot)
          // if we can sneak ahead one character safely, preload next paramCounter
          if(i + 1 < inputPos)
          {
            parameters[paramCounter] = &inputLine[i + 1];
          }
        }
      }


#ifdef DEBUG
      cout.println("handle command 0");
#endif

      handleCommand(Parameters(parameters, paramCounter + 1, this));

#ifdef DEBUG
      cout.println("handle command 1");
#endif

      cout.println();

      showPrompt();

      inputPos = 0;
    }
    else
    {
#ifdef FACT_LIB_STRICT
      if(inputPos == CONSOLE_INPUTLINE_MAX - 1)
      {
#ifdef DEBUG
        cout << F("Max len");
#else
        cout << (char)7; // old ASCII beep
#endif
        return;
      }
#endif
      cout << received;
      inputLine[inputPos++] = received;
    }
  }
}


void ConsoleMenuHandler::handleCommand(Parameters p)
{
#ifdef DEBUG
  cout << F("handle command deep: ") << breadCrumbPos;
  cout.println();
  cout << F("  menu ptr = ") << (uint32_t)getActiveMenu();
  cout.println();
#endif

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

void MenuEnumerator::add(Menu& menu, const __FlashStringHelper* name, const __FlashStringHelper* description)
{
  menu.setDesc(name, description);
  menus.add(&menu);
}

Menu* MenuEnumerator::canHandle(IMenu::Parameters p)
{
#ifdef DEBUG2
  cout.println("MenuEnumerator::canHandle");
#endif

  Menu* menu = (Menu*) menus.getHead();

  while(menu != NULL)
  {
    if(menu->canHandle(p))
      return menu;

    menu = (Menu*) menu->getNext();
  }

  return NULL;
}
