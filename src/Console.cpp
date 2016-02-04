#include "Console.h"

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
