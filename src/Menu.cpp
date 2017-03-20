#include "Console.h"
#include "fact/Menu.h"

using namespace FactUtilEmbedded;
#ifndef FEATURE_IOSTREAM
using namespace FactUtilEmbedded::std;
#else
using namespace std;
#endif

// This is when we do string arithmatic/reference already-existing F() strings
#define FLASHSTRING(str) (reinterpret_cast<const __FlashStringHelper *>(str))

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
  clog << "Menu::canHandle entry";
#endif

  return MenuEnumerator::canHandle(p);
}

void Menu::handleCommand(Parameters p)
{
  MENU_DECLARE_COUT;

#ifdef DEBUG2
  clog << "Menu::handle command";
#endif

  MenuCommand* menu = canHandle(p);

#ifdef DEBUG2
  clog << "Menu::handle command 2";
#endif

  if(menu != NULL)
    menu->handleCommand(p.inc());
  else if(strcmp_P(*p.parameters, PSTR("help")) == 0)
  {
    showHelp(p.inc());
  }
  else
  {
    out << endl;
    out << F(ERROR_UNRECOGNIZED_COMMAND) << *p.parameters;
    out << endl;
  }
}


void Menu::showHelp(Parameters p)
{
  MENU_DECLARE_COUT;

  if(p.count == 0)
  {
    SinglyLinkedNode* node = getHeadMenu();

    out << endl;

#ifndef MEMORY_OPT_CODE
    out << F("Fact util v") << F(FACT_UTIL_EMBEDDED_VERSION);
    out << endl;

    if(node == NULL)
    {
      out << F("No commands registered");
      return;
    }

#endif

    out << F(INFO_AVAILABLE_COMMAND);

    out << endl;
    out << endl;

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

      out << F("  ");

      const __FlashStringHelper* name = menu->getName();
      const __FlashStringHelper* description = menu->getDescription();

#ifdef DEBUG
      if(name == NULL) { clog << F("Name=NULL"); continue; }
      if(description == NULL) { clog << F("Desc=NULL"); continue; }
#endif
      showKeyValuePair(out, name, description, width);
      out << endl;

#ifdef DEBUG
      counter++;
#endif
    }

#ifdef DEBUG
    clog << endl;
    clog << (uint16_t)counter << F(" item(s) shown");
    clog << endl;
#endif
  }
}



#if defined(CONSOLE_FEATURE_AUTOCOMPLETE)
bool Menu::processInput(Console* console, char received)
{
  fact_ostream& out = console->getOut();

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

      // only render the suffix, not the whole command again,
      // since user typed in prefix already
      commandName += inputPos;

#ifdef ESP8266
      // FIX: I believe we can replace FPSTR with FLASHSTRING on ESP8266 for our
      // usage scenarios
      // but not easy to test right now
      out << FPSTR(commandName);
#else
      out << FLASHSTRING(commandName);
#endif
      console->appendToInputLine_P(commandName);
    }
    else if(candidate_count > 1)
    {
      layer1::LinkedListIterator<MenuBase> i = getIterator();

      out << endl;

      for(; i; i++)
      {
        auto commandName = i()->getName();

        // if the first characters of input match the command-
        if(strncmp_P(inputLine, (PGM_P) commandName, inputPos) == 0)
          out << commandName << ' ';
      }

      out << endl;
      console->showPrompt();
      out << console->getInputLine();
    }
    return true;
  }
  else
    return false;
}
#endif
