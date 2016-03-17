#include "Console.h"
#include "fact/Menu.h"

using namespace FactUtilEmbedded;


#ifdef MEMORY_OPT_CODE
#define ERROR_UNRECOGNIZED_COMMAND "Unrecognized: "
#define INFO_AVAILABLE_COMMAND "Available: "
#else
#define ERROR_UNRECOGNIZED_COMMAND "Unrecognized command: "
#define INFO_AVAILABLE_COMMAND "Following commands are available: "
#endif



MenuCommand* Menu::canHandle(Parameters p)
{
#ifdef DEBUG2
  cout.println("Menu::canHandle entry");
#endif

  return MenuEnumerator::canHandle(p);
}

void Menu::handleCommand(Parameters p)
{
#ifdef DEBUG2
  cout.println("Menu::handle command");
#endif

  MenuCommand* menu = canHandle(p);

#ifdef DEBUG2
  cout.println("Menu::handle command 2");
#endif

  if(menu != NULL)
    menu->handleCommand(p.inc());
  else if(strcmp_P(*p.parameters, PSTR("help")) == 0)
  {
    showHelp(p.inc());
  }
  else
  {
    cout.println();
    cout << F(ERROR_UNRECOGNIZED_COMMAND) << *p.parameters;
    cout.println();
  }
}


void Menu::showHelp(Parameters p)
{
  if(p.count == 0)
  {
    SinglyLinkedNode* node = getHeadMenu();

    cout.println();

#ifndef MEMORY_OPT_CODE
    if(node == NULL)
    {
      cout << F("No commands registered");
      return;
    }
#endif

    cout << F(INFO_AVAILABLE_COMMAND);

    cout.println();
    cout.println();

    for(; node != NULL; node = node->getNext())
    {
      auto menu = (MenuBase*) node;

      cout << F("  ");

      const __FlashStringHelper* name = menu->getName();
      const __FlashStringHelper* description = menu->getDescription();

#ifdef DEBUG
      if(name == NULL) { cout.println(F("Name=NULL")); continue; }
      if(description == NULL) { cout.println(F("Desc=NULL")); continue; }
#endif
      showKeyValuePair(name, description, 16);
      cout.println();
    }
  }
}



#if defined(CONSOLE_FEATURE_AUTOCOMPLETE)
bool Menu::processInput(Console* console, char received)
{
  // look for tab character
  if(received == 9)
  {
    char* inputLine = console->getInputLine();
    uint8_t inputPos = console->getInputPos();

    SinglyLinkedNode* node = getHeadMenu();
    for(; node != NULL; node = node->getNext())
    {
      auto menu = (MenuBase*) node;
      auto commandName = (PGM_P) menu->getName();
      if(strncmp_P(inputLine, commandName, inputPos) == 0)
      {
        cout << (const __FlashStringHelper*)(commandName + inputPos);
        console->appendToInputLine_P(commandName + inputPos);
      }
    }
    return true;
  }
  else
    return false;
}
#endif
