#include "Console.h"
#include "fact/Menu.h"

using namespace FactUtilEmbedded;



void MenuCommand::showPrompt(Console* console)
{
#ifdef DEBUG3
  cout.println(F("Menu::showPrompt"));
#endif

  if(getName() != NULL) console->getOut() << getName();

#ifdef DEBUG3
  cout.println(F("Menu::showPrompt exit"));
#endif
}


IMenu* MenuCommand::canHandle(IMenu::Parameters p)
{
  const __FlashStringHelper* name = getName();

#ifdef DEBUG2
  cout.println(F("Menu::canHandle"));
  cout << F("evaluating ") << p.parameters[0] << F(" against name: ") << name;
  cout.println();
#endif
#if defined(DEBUG3) && defined(ESP8266) && defined(UNUSED)
  static int __result;
  PGM_P p2 = reinterpret_cast<PGM_P>(name);
  cout << F("ptrs. orig=") << (uint32_t) name << F(" converted: ") << (uint32_t) p2;
  cout << F(" ptrs. param=") << (uint32_t) p.parameters[0];
  cout.println();
  const __FlashStringHelper* _test1 = F("status2");
  char* _test2 = "abc";
  cout << F("phase 1:");
  auto _result = strcmp_P(_test2, (PGM_P)_test1);
  cout << _result << F(",phase 2:");
  _result = strncmp_P_dbg(_test2, (PGM_P)name, 3);
  cout << _result << F(",phase 3:");
  _result = strcmp_P_dbg(_test2, (PGM_P)name);
  cout << _result << F(",phase 4:");
  _result = strcmp_P_dbg(p.parameters[0], (PGM_P) name);
  cout << _result << F(",phase 5:");
  cout.println(F("Done testing"));
#endif


  if(strcmp_P(p.parameters[0], (PGM_P) name) == 0)
  {
#ifdef DEBUG3
    cout << F("Exit this");
#endif
    return this;
  }
  else
  {
#ifdef DEBUG3
    cout << F("Exit NULL");
#endif
    return NULL;
  }
}
