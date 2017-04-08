#include "Console.h"

/*
#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif
*/

using namespace FactUtilEmbedded;


void ConsoleMenu::handleCommand(Parameters p)
{
#ifdef DEBUG2
    cout << F("ConsoleMenu::handleCommand: ") << breadCrumbPos;
    cout.println();
    cout << F("  menu ptr = ") << (uint32_t)getActiveMenu();
    cout.println();
#endif

    getActiveMenu()->handleCommand(p);
}


void ConsoleMenu::showPrompt()
{
    fact_ostream &out = getOut();

    for (int i = 0; i < breadCrumbPos; i++)
    {
#ifdef DEBUG2
        cout << F("ConsoleMenu::showPrompt: ") << i;
        cout.println();
#endif
        breadCrumb[i]->showPrompt(this);
        if (i != (breadCrumbPos - 1)) out << ' ';
    }

    out << F("> ");
}

#if defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)

bool ConsoleMenu::processInput(char received)
{
    return getActiveMenu()->processInput(this, received);
}

#endif
