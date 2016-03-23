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
    cout << F("Fact util v") << F(FACT_UTIL_EMBEDDED_VERSION);
    cout.println();

    if(node == NULL)
    {
      cout << F("No commands registered");
      return;
    }

#endif

    cout << F(INFO_AVAILABLE_COMMAND);

    cout.println();
    cout.println();

#ifdef DEBUG
    uint8_t counter = 0;
#endif

#ifndef MEMORY_OPT_CODE
    uint8_t width = 0;

    // auto-detect width of key
    for(; node != NULL; node = node->getNext())
    {
      auto menu = (MenuBase*) node;
      uint8_t len = strlen_P((PGM_P)menu->getName());

      if(len > width)  width = len;
    }

    width++;

    node = getHeadMenu();
#else
    static const uint8_t width = 16;
#endif

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
      showKeyValuePair(name, description, width);
      cout.println();

#ifdef DEBUG
      counter++;
#endif
    }

#ifdef DEBUG
    cout.println();
    cout << counter << F(" item(s) shown");
    cout.println();
#endif
  }
}



#if defined(CONSOLE_FEATURE_AUTOCOMPLETE)
bool Menu::processInput(Console* console, char received)
{
  // look for tab character
  if(received == 9)
  {
    // see what current input line looks like
    char* inputLine = console->getInputLine();

    // see what position we're at
    uint8_t inputPos = console->getInputPos();

    // now iterate thru all the menus
    //SinglyLinkedNode* node = getHeadMenu();

    MenuBase* candidate = NULL;
    unsigned candidate_count = 0;

    layer1::LinkedListIterator<MenuBase> i = getIterator();

    // trying to get 1 and only 1 to autocomplete.  If many
    // candidates show up ( > 1) abort and go into linux-
    // style multi candidate select mode
    for(; i && candidate_count < 2; i++)
    //for(; node != NULL; node = node->getNext())
    {
      //auto menu = (MenuBase*) node;
      //auto commandName = (PGM_P) menu->getName();
      auto commandName = (PGM_P) i.getCurrent()->getName();
      
      // if the first characters of input match the command-
      if(strncmp_P(inputLine, commandName, inputPos) == 0)
      {
        candidate_count++;
        candidate = i;
      }
    }

    if(candidate_count == 1)
    {
      auto commandName = (PGM_P) candidate->getName();

      cout << FPSTR((commandName + inputPos));
      console->appendToInputLine_P(commandName + inputPos);
    }
    else if(candidate_count > 1)
    {
      layer1::LinkedListIterator<MenuBase> i = getIterator();

      cout.println();

      for(; i; i++)
      {
        auto commandName = (PGM_P) i.getCurrent()->getName();

        // if the first characters of input match the command-
        if(strncmp_P(inputLine, commandName, inputPos) == 0)
          cout << FPSTR(commandName) << ' ';
      }

      cout.println();
      console->showPrompt();
      cout << console->getInputLine();
    }
    return true;
  }
  else
    return false;
}
#endif
