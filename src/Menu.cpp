#include "Console.h"
#include "fact/Menu.h"
//#include "Service.h"

using namespace FactUtilEmbedded;


#ifdef MEMORY_OPT_CODE
#define ERROR_UNRECOGNIZED_COMMAND "Unrecognized: "
#define INFO_AVAILABLE_COMMAND "Available: "
#else
#define ERROR_UNRECOGNIZED_COMMAND "Unrecognized command: "
#define INFO_AVAILABLE_COMMAND "Following commands are available: "
#endif


#ifndef AVR
// samd is supposed to have a strncmp_P mapping to regular strncmp,
// but doesn't for some reason (probably platformio outdated) so do it here
#define strncmp_P(s1, s2, n) strncmp((s1), (s2), (n))
#endif


Menu* MenuHandler::canHandle(Parameters p)
{
#ifdef DEBUG
  cout.println("MenuHandler::canHandle entry");
#endif

  return MenuEnumerator::canHandle(p);
}

void MenuHandler::handleCommand(Parameters p)
{
#ifdef DEBUG
  cout.println("MenuHandler::handle command");
#endif

  Menu* menu = canHandle(p);

#ifdef DEBUG
  cout.println("MenuHandler::handle command 2");
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


void MenuHandler::showHelp(Parameters p)
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
      Menu* menu = (Menu*) node;

      cout << F("  ");
      showKeyValuePair(menu->getName(), menu->getDescription(), 16);
      cout.println();
    }
  }
}



#if defined(CONSOLE_FEATURE_AUTOCOMPLETE) && defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)
bool MenuHandler::processInput(Console* console, char received)
{
  // look for tab character
  if(received == 9)
  {
    char* inputLine = console->getInputLine();
    uint8_t inputPos = console->getInputPos();

    SinglyLinkedNode* node = getHeadMenu();
    for(; node != NULL; node = node->getNext())
    {
      Menu* menu = (Menu*) node;
      const char* commandName = (const char*) menu->getName();
      if(strncmp_P(inputLine, commandName, inputPos) == 0)
      {
        // TODO: make an appendToInputLine_P
        char temp[32];

        strcpy_P(temp, commandName + inputPos);

        cout << temp;

        console->appendToInputLine(temp);
      }
    }
    return true;
  }
  else
    return false;
}
#endif


void Menu::showPrompt()
{
  cout << name;
}

IMenu* Menu::canHandle(IMenu::Parameters p)
{
#ifdef DEBUG2
  cout.println("Menu::canHandle");
  cout << F("evaluating ") << p.parameters[0] << F(" against name: ") << name;
  cout.println();
#endif
#ifdef DEBUG3
  PGM_P p2 = reinterpret_cast<PGM_P>(name);
  cout << F("ptrs. orig=") << (uint32_t) name << F(" converted: ") << (uint32_t) p2;
  cout.println();
  cout << F("Converted to: ") << p2 << "...";
  cout.println();
  auto result = strcmp_P(p.parameters[0], p2);
  cout << F("Result = ") << result;
#endif

  
  if(strcmp_P(p.parameters[0], (const char*) name) == 0)
    return this;
  else
    return NULL;
}

void MenuGeneric::handleCommand(IMenu::Parameters p)
{
#ifdef DEBUG2
  cout.println("MenuGeneric::handleCommand");
#endif

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

void NestedMenuHandler::handleCommand(Parameters p)
{
  if(strcmp_P(*p.parameters, PSTR("cd")) == 0)
  {
    IMenu* handleAble = canHandle(p.inc());

    if(handleAble)
    {
      selected = handleAble;
    }
  }
}

void NestedMenuHandler::showPrompt()
{
  // TODO: We can turn getName into showLocalPrompt if we want to go even more OOP
  cout << getName();
  if(getSelected())
  {
    cout << ' ';
    getSelected()->showPrompt();
  }
  else
  {
    cout << '>';
  }
}
