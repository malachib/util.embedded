#include "fact/Menu.h"

using namespace FactUtilEmbedded;


void MenuGeneric::handleCommand(IMenu::Parameters p)
{
#ifdef DEBUG2
  cout.println("MenuGeneric::handleCommand");
#endif

  handler(p);
}

// TODO: Switch this to PGM_P
void IMenu::_showKeyValuePair(
#ifdef CONSOLE_FEATURE_COUT
  Stream& out,
#endif
  const __FlashStringHelper* key, uint8_t keyPadding)
{
#ifndef CONSOLE_FEATURE_COUT
  auto out = cout;
#endif

  // FIX: some Print classes don't seem to return proper bytes-written
  size_t nameLength = strlen_P((const char*) key);
  //size_t nameLength = cout.print(menu->getName());
  out.print(key);
  if(keyPadding > nameLength)
    keyPadding -= nameLength;

  while(keyPadding-- > 0) out.print(' ');
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
