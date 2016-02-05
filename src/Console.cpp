#include "Console.h"

using namespace FactUtilEmbedded;

void Console::handler()
{
  while(cin.available() > 0)
  {
    char received = cin.read();
    cout << received;

    // newline received, actually perform action
    if(received == '\n' || received == 13)
    {
      int paramCounter = 0;
      char* parameters[10];

      inputLine[inputPos] = 0;

      // No input = just show prompt again
      if(inputPos == 0)
      {
        cout.println();
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

      handleCommand(Parameters(parameters, paramCounter + 1));

      cout.println();

      showPrompt();

      inputPos = 0;
    }
    else
      inputLine[inputPos++] = received;
  }
}


void ConsoleMenuHandler::handleCommand(Parameters p)
{
  Menu* menu = canHandle(p);
  if(menu != NULL)
    menu->handleCommand(p);
  else if(strcmp_P(*p.parameters, PSTR("help")) == 0)
  {
    showHelp(p.inc());
  }
  else
  {
    cout.println();
    cout << F("Unrecognized command: ") << *p.parameters;
    cout.println();
  }
}


void ConsoleMenuHandler::showHelp(Parameters p)
{
  if(p.count == 0)
  {
    SinglyLinkedNode* node = getHeadMenu();

    cout.println();

    if(node == NULL)
    {
      cout << F("No commands registered");
      return;
    }

    cout << F("Following commands are available: ");

    cout.println();
    cout.println();

    for(; node != NULL; node = node->getNext())
    {
      Menu* menu = (Menu*) node;

      cout << F("  ");
      size_t nameLength = cout.print(menu->getName());
      uint8_t paddingLength = 25 - nameLength;
      while(paddingLength > 0) cout.print(' ');
      cout.print(menu->getDescription());
      cout.println();
    }
  }
}


void ConsoleMenuHandler::showPrompt()
{
  for(int i = 0; i < breadCrumbPos; i++)
  {
    breadCrumb[i]->showPrompt();
    if(i != (breadCrumbPos - 1)) cout << F(" ");
  }

  cout << F("> ");
}


void MenuEnumerator::add(Menu& menu)
{
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


void Menu::showPrompt()
{
  cout << name;
}

IMenu* Menu::canHandle(IMenu::Parameters p)
{
  if(strcmp_P(p.parameters[0], (const char*) name) == 0)
    return this;
  else
    return NULL;
}
