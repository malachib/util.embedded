// exists to address GitHub Issue#3

#include <Arduino.h>

#include "features.h"

#ifdef FEATURE_IPC
#include <fact/ipc.h>
#endif
#ifdef FEATURE_CONSOLE
#include <fact/Menu.h>
#include <fact/MenuFunction.h>
#endif
#include <Console.h>
#include <fact/lib.h>

using namespace util;


void testFunc1(int);
void testFunc2(char);

#ifdef FEATURE_CONSOLE
Menu menu;
ConsoleMenu console(&menu);
#endif

#ifdef FEATURE_IPC
union TEST
{
  // we use unsigned char because it allows the crazy casting we want to do
  unsigned char* buffer[64];
  //IInvoker invoker; // can't do this because IInvoker is an abstract class, but in effect we are doing this
} _TEST;

// presumably unions are already volatile?
//volatile TEST _TEST;

void testInvoker()
{
  auto f = IPCHelper::create(testFunc1, 3);
  memcpy(_TEST.buffer, &f, sizeof(f));
  //reinterpret_cast<IInvoker*>(&_TEST.buffer)->invoke();
  ((IInvoker*)&_TEST.buffer)->invoke();
}
#endif

#ifdef FEATURE_IPC2
void testInvoker2()
{
  auto f = IPCHelper::create(testFunc2, 'a');
  memcpy(_TEST.buffer, &f, sizeof(f));
  //reinterpret_cast<IInvoker*>(&_TEST.buffer)->invoke();
  ((IInvoker*)&_TEST.buffer)->invoke();
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
CREATE_MENUFUNCTION(menuDirect, testFunc1, "Directly invoke function");
#endif

void setup()
{
  Serial.begin(115200);
  Serial << F("Starting up");
  Serial << (int) 1; // integer rendering takes us from 4084 to 4490, 406 bytes
  //Serial << 'a';   // character rendering already present
  Serial.println();
  
#ifdef FEATURE_CONSOLE
#ifdef FEATURE_IPC
  menu.add(menuInvoker);
#endif
#ifdef FEATURE_IPC2
  menu.add(menuInvoker2);
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
#endif
}


void loop()
{
#ifdef FEATURE_CONSOLE
  console.handler();
#endif
}
