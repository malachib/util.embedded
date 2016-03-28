#include "Console.h"

/*
#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif
*/

using namespace FactUtilEmbedded;

// technically this is a stack opt
#ifdef MEMORY_OPT_DATA
#define CONSOLE_BEHAVIOR_MAX_PARAMETER 4
#else
#define CONSOLE_BEHAVIOR_MAX_PARAMETER 8
#endif

#ifndef CONSOLE_FEATURE_COUT
#define in cin
#endif


// TODO: refactor code so that handler is an inline call and processInput
// contains all the smarts (not inline call).  This way, we can enable/disable
// CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR feature from an #include rather than
// a -D switch
void Console::handler()
{
  while(in.available() > 0)
  {
    Stream& out = getOut();
    char received = in.read();

    if(processInput(received))
    {

    }
    else
    if(received == '\n' || received == 13)
    {
      out.println();

      int paramCounter = 0;
      const char* parameters[CONSOLE_BEHAVIOR_MAX_PARAMETER];

      inputLine[inputPos] = 0;

      // No input = just show prompt again
      if(inputPos == 0)
      {
        showPrompt();
        return;
      }

#ifdef DEBUG2
      cout << F("Submitting command: ") << inputLine << F("\n");
#endif

      parameters[0] = inputLine;

      for(int i = 0; i < inputPos; i++)
      {
        if(inputLine[i] == ' ')
        {
          inputLine[i] = 0;
#ifdef DEBUG2
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


#ifdef DEBUG2
      cout.println("handle command 0");
#endif

      handleCommand(Parameters(parameters, paramCounter + 1, this));

#ifdef DEBUG2
      cout.println("handle command 1");
#endif

      out.println();

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
        out << (char)7; // old ASCII beep
#endif
        return;
      }
#endif
      out << received;
      inputLine[inputPos++] = received;
    }
  }
}


// remember we don't bother will null termination for inputLine since
// we're already maintaining inputPos
void Console::appendToInputLine_P(PGM_P src)
{
  char ch;

  // apparently strlcpy_P isn't available for some platforms
  //inputPos += strlcpy_P(&inputLine[inputPos], src, CONSOLE_INPUTLINE_MAX - inputPos);
  while((ch = pgm_read_byte(src++)) != 0) inputLine[inputPos++] = ch;

}
