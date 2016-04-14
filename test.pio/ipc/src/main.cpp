// exists to address GitHub Issue#3

#include <Arduino.h>

#include "features.h"

#ifdef FEATURE_IPC
#include <fact/rpc.h>
#endif
#ifdef FEATURE_CONSOLE
#include <fact/Menu.h>
#include <fact/MenuFunction.h>
#endif
#include <Console.h>
#include <fact/lib.h>

using namespace util;
using namespace util::rpc;


void testFunc1(int);
void testFunc2(char);

#ifdef FEATURE_CONSOLE
Menu menu;
ConsoleMenu console(&menu);
#endif

#ifdef FEATURE_IPC
// TODO: would like to ensure _TEST starts on a proper boundary
union TEST
{
  // we use unsigned char because it allows the crazy casting we want to do
  uint8_t buffer[16];
  //IInvoker invoker; // can't do this because IInvoker is an abstract class, but in effect we are doing this
} _TEST;

// presumably unions are already volatile?
//volatile TEST _TEST;

void testInvoker()
{
  auto f = CallHolderFactory::create(testFunc1, 3);
  //f.invoke();
  memcpy(_TEST.buffer, &f, sizeof(f));
  //reinterpret_cast<IInvoker*>(&_TEST.buffer)->invoke();
  ((IInvoker*)&_TEST)->invoke();
}
#endif

#ifdef FEATURE_IPC2
void testInvoker2()
{
  auto f = CallHolderFactory::create(testFunc2, 'a');
  memcpy(&_TEST.buffer, &f, sizeof(f));
  //reinterpret_cast<IInvoker*>(&_TEST.buffer)->invoke();
  ((IInvoker*)&_TEST)->invoke();
}
#endif


#ifdef FEATURE_IPC3
void testInvoker3()
{
  auto f = CallHolderFactory::create(testFunc2, 'b');
  memcpy(&_TEST.buffer, &f, sizeof(f));
  //reinterpret_cast<IInvoker*>(&_TEST.buffer)->invoke();
  ((IInvoker*)&_TEST)->invoke();
}
#endif


void testFunc1(int value)
{
  cout << F("I got it: ") << value;
  cout.println();
}

void testFunc2(char value)
{
  cout << F("I got it: ") << value;
  cout.println();
}



#ifdef FEATURE_CONSOLE
#ifdef FEATURE_IPC
CREATE_MENUFUNCTION(menuInvoker, testInvoker, "Test out CallHolder class");
#endif
#ifdef FEATURE_IPC2
CREATE_MENUFUNCTION(menuInvoker2, testInvoker2, "Test out CallHolder class");
#endif
#ifdef FEATURE_IPC3
CREATE_MENUFUNCTION(menuInvoker3, testInvoker3, "Test out CallHolder class");
#endif
CREATE_MENUFUNCTION(menuDirect, testFunc1, "Directly invoke function");
#endif

void setup()
{
  cout.begin(115200);
  cout << F("Starting up");
  cout << (int) 1; // integer rendering takes us from 4084 to 4490, 406 bytes
  //Serial << 'a';   // character rendering already present
  cout.println();
  
#ifdef FEATURE_CONSOLE
#ifdef FEATURE_IPC
  menu.add(menuInvoker);
#endif
#ifdef FEATURE_IPC2
  menu.add(menuInvoker2);
#endif
#ifdef FEATURE_IPC3
  menu.add(menuInvoker3);
#endif
  menu.add(menuDirect);
#endif

#ifndef FEATURE_CONSOLE
  delay(5000);
#if defined(FEATURE_IPC)
  testInvoker();
#endif
#ifdef FEATURE_IPC2
  testInvoker2();
#endif
#ifdef FEATURE_IPC3
  testInvoker3();
#endif
#endif
}


void loop()
{
#ifdef FEATURE_CONSOLE
  console.handler();
#endif
}
