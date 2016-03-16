//--------------------------------------------------------------------
// Arduino Console Stub
//--------------------------------------------------------------------

//#if _MSC_VER

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Add CPU hardware definitions

#define _AVR_IO_H_
#define F(str)	str
#define PSTR(str) (str)
#define _SFR_IO8(io_addr) ((io_addr))
//#include "C:\Program Files (x86)\Arduino\hardware\tools\avr\avr\include\avr\iomxx0_1.h"

#define boolean bool
#define byte char

// From "c:\program files\arduino\hardware\cores\arduino\print.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

// From "c:\program files\arduino\hardware\cores\arduino\wiring.h"

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1

#define PI 3.14159265
#define HALF_PI 1.57079
#define TWO_PI 6.283185
#define DEG_TO_RAD 0.01745329
#define RAD_TO_DEG 57.2957786

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define INTERNAL 3
#define DEFAULT 1
#define EXTERNAL 0

#define PROGMEM

typedef void __FlashStringHelper;
#define PGM_P  const char *

class Print
{
	void printf(const char* format ...);

public:
	void begin(long);

	void print(bool);
	void print(char);
	void print(unsigned char);
	void print(const char*);
	void print(int, int);
	void print(unsigned short);
	void print(unsigned int);
	void print(unsigned long);
	void print(int);
	void print(long);
	void print(const __FlashStringHelper* s)
	{ print((const char*) s); }

	void println();
	void println(char*);
	void println(const char*); // to satisfy C++ 11 dislike of string literal to char*
	//void println(bool);
	void println(int, int);
	void println(unsigned int, int);
	void println(unsigned long, int);
	int available();
	char read();
	virtual size_t write(const uint8_t *buffer, size_t size) = 0;
	size_t write(const char *buffer, size_t size) {
			return write((const uint8_t *) buffer, size);
	}
	// VSPDE
	void _append(char c);
private:
	char buffer[1024];
	int buflen;
};

class Stream : public Print
{
public:
	virtual int available() = 0;
	virtual int read() = 0;
};


class IOStream : public Stream
{
public:
	virtual int available() override;
	virtual int read() override;
	virtual size_t write(const uint8_t* bytes, size_t len) override;
};

class NCursesStream : public Stream
{
public:
	NCursesStream();

	virtual int available() override;
	virtual int read() override;
	virtual size_t write(const uint8_t* bytes, size_t len) override;
};

#ifdef NCURSES
extern NCursesStream Serial;
#else
extern IOStream Serial;
#endif
extern unsigned long millis();
extern unsigned long micros();
extern void delay(unsigned long);
extern void pinMode(int, int);
extern void analogWrite(int, int);
extern int analogRead(int);
extern void digitalWrite(int, int);
extern bool digitalRead(int);

extern void tone(int, int);
extern void tone(int, int, int);
extern void noTone(int);

extern void yield();

// all these pgm space stuff lifted straight out of MPU6050 headers (thanks!)
#define strcpy_P(dest, src) strcpy((dest), (src))
#define strcat_P(dest, src) strcat((dest), (src))
#define strcmp_P(a, b) strcmp((a), (b))
#define strncmp_P(s1, s2, n) strncmp((s1), (s2), (n))
#define strlen_P(a) strlen(a)

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_float(addr) (*(const float *)(addr))

#define pgm_read_byte_near(addr) pgm_read_byte(addr)
#define pgm_read_word_near(addr) pgm_read_word(addr)
#define pgm_read_dword_near(addr) pgm_read_dword(addr)
#define pgm_read_float_near(addr) pgm_read_float(addr)
#define pgm_read_byte_far(addr) pgm_read_byte(addr)
#define pgm_read_word_far(addr) pgm_read_word(addr)
#define pgm_read_dword_far(addr) pgm_read_dword(addr)
#define pgm_read_float_far(addr) pgm_read_float(addr)


typedef void(*_debugWrite)(char *msg);

extern _debugWrite debugWrite;

//#endif
