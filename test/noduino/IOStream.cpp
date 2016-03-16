// standin lib redirecting arduino-style Serial to stdin/stdout

#include "vspde.h"
#include <ncurses.h>


//using namespace std;

IOStream Serial;

IOStream::IOStream()
{
  initscr();
  noecho();
  //nocbreak();
  cbreak();

  printw("Initializing...1");
  printw("\nInitializing...2\n\r");
  //printw("\rInitializing...3");
}

int lastChar = -1;

int IOStream::read()
{
  if(lastChar != -1)
  {
    int c = lastChar;
    lastChar = -1;
    return c;
  }
  nodelay(stdscr, FALSE);
  //timeout(-1);
  //return cin.get();
  return getch();
}

int IOStream::available()
{
  int c;

  if(lastChar != -1) return 1;

  //timeout(0);
  nodelay(stdscr, TRUE);

  //return cin.tellg();
  //return cin.rdbuf()->in_avail();
  //return (cin.peek() & ios_base::io_state::eofbit) ? 1 : 0;
  //return cin.peek() == EOF ? 0 : 1;
  if ((c = getch()) == ERR)
    return 0;
  else
  {
    lastChar = c;
    return 1;
  };
}


size_t IOStream::write(const uint8_t* bytes, size_t len)
{
  while(len--)
  {
    auto c = *bytes++;

    if(c == 13) continue;
    if(c == 10)
    {
      addch(10);
      addch(13);
      continue;
    }
    // if we get \r\n it blanks out the line -- but \n\r is OK, so eat the \r
    // and whenever we get an \n spit out \n\r.  ugly hack , but that's life
    addch(c);
  }
  //printw((char*)bytes);
  //printw(bytes);
  //refresh();
  doupdate();
}
