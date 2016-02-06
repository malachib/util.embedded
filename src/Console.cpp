#include "Console.h"

#ifndef AVR
// samd is supposed to have a strncmp_P mapping to regular strncmp,
// but doesn't for some reason (probably platformio outdated) so do it here
#define strncmp_P(s1, s2, n) strncmp((s1), (s2), (n))
#endif

using namespace FactUtilEmbedded;


void Console::handler()
{
  while(cin.available() > 0)
  {
    char received = cin.read();

#ifdef CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR
    if(processInput(received))
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

      handleCommand(Parameters(parameters, paramCounter + 1));

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

Menu* MenuHandler::canHandle(Parameters p)
{
  return MenuEnumerator::canHandle(p);
}

void MenuHandler::handleCommand(Parameters p)
{
  Menu* menu = canHandle(p);
  if(menu != NULL)
    menu->handleCommand(p.inc());
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


void MenuHandler::showHelp(Parameters p)
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
      showKeyValuePair(menu->getName(), menu->getDescription(), 16);
      cout.println();
    }
  }
}

void ConsoleMenuHandler::handleCommand(Parameters p)
{
  breadCrumb[breadCrumbPos - 1]->handleCommand(p);
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
bool ConsoleMenuHandler::processInput(char received)
{
  // look for tab character
  if(received == 9)
  {
    char* inputLine = getInputLine();

    SinglyLinkedNode* node = getHeadMenu();
    for(; node != NULL; node = node->getNext())
    {
      Menu* menu = (Menu*) node;
      const char* commandName = (const char*) menu->getName();
      if(strncmp_P(inputLine, commandName, getInputPos()) == 0)
      {
        // TODO: make an appendToInputLine_P
        char temp[32];

        strcpy_P(temp, commandName + getInputPos());

        cout << temp;

        appendToInputLine(temp);
      }
    }
    return true;
  }
  else
    return false;
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

void MenuGeneric::handleCommand(IMenu::Parameters p)
{
  handler(p);
}

void IMenu::showKeyValuePair(const __FlashStringHelper* key, const __FlashStringHelper* value, uint8_t keyPadding)
{
  // FIX: some Print classes don't seem to return proper bytes-written
  size_t nameLength = strlen_P((const char*) key);
  //size_t nameLength = cout.print(menu->getName());
  cout.print(key);
  if(keyPadding > nameLength)
    keyPadding -= nameLength;

  while(keyPadding-- > 0) cout.print(' ');
  cout.print(value);
}
