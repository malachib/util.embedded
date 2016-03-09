//--------------------------------------------------------------------
// Arduino Console Stub
//--------------------------------------------------------------------

//#if _MSC_VER

#include "vspde.h"
#if _MSC_VER
#include <conio.h>
#define USE_ULTOA 1
#endif
#include <time.h>

#ifndef USE_ULTOA
#include <string>
#endif

extern void setup();
extern void loop();

_debugWrite debugWrite;

Print Serial;

//--------------------------------------------------------------------
// Timers
//--------------------------------------------------------------------

unsigned long millis()
{
	return (clock() * 1000) / CLOCKS_PER_SEC;
}

unsigned long micros()
{
	return clock() / CLOCKS_PER_SEC;
}

void delay(unsigned long delayms)
{
	unsigned long u = millis() + delayms;
	while (u > millis())
		yield();
}

void yield()
{
	// context switch here if we can
}

//--------------------------------------------------------------------
// I/O
//--------------------------------------------------------------------

void pinMode(int, int)
{
}

void digitalWrite(int, int)
{
}

bool digitalRead(int)
{
	return 0;
}


void analogWrite(int, int)
{

}

int analogRead(int)
{
    return 0;
}

//--------------------------------------------------------------------
// Serial
//--------------------------------------------------------------------

void Print::begin(long)
{
	buffer[0] = 0;
	buflen = 0;
}

void Print::print(const char *pString)
{
    if (debugWrite != NULL)
    {
        debugWrite((char*)pString);
    }
    else
	    printf("%s", pString);
}

void Print::print(int value, int)
{
	printf("%d", value);
}


void Print::print(unsigned short value)
{
    print((unsigned int)value);
}


void Print::print(unsigned int value)
{
    print((unsigned long)value);
}


void Print::print(unsigned long value)
{
#if USE_ULTOA
    char buffer[10];
    ultoa(value, buffer, 10);
#else
		std::string _buffer = std::to_string(value);
		const char* buffer = _buffer.c_str();
#endif

    print(buffer);
}


void Print::print(long value)
{
		std::string _buffer = std::to_string(value);
		const char* buffer = _buffer.c_str();

    print(buffer);
}

void Print::print(int value) 
{
	print((long)value);
}

void Print::print(char value)
{
	printf("%c", value);
}


void Print::print(unsigned char value)
{
    print((unsigned long)value);
}


void Print::print(bool value)
{
	print(value ? "true" : "false");
}


void Print::println()
{
    print("\r\n");
}

void Print::println(char *pString)
{
    print(pString);
    println();
}

void Print::println(const char *pString)
{
    print(pString);
    println();
}


void Print::println(int value, int)
{
	printf("%d\r\n", value);
}

void Print::println(unsigned int value, int)
{
	printf("%u\r\n", value);
}

void Print::println(unsigned long value, int)
{
	printf("%lu\r\n", value);
}


int Print::available()
{
	return buflen;
}

char Print::read()
{
	char c = 0;
	if (buflen > 0)
	{
		c = buffer[0];
		memcpy(&buffer[0], &buffer[1], --buflen);
	}
	return c;
}
void Print::_append(char c)
{
	Print::buffer[buflen] = c;
	if (++buflen >= 1024)
	{
		buflen--;
	}
}

//#endif
