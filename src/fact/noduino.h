//--------------------------------------------------------------------
// For non-arduino environments, this file exists only to support
// mostly-hardware-unaware libs within util.embedded
//--------------------------------------------------------------------

//#if _MSC_VER

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#if !defined(ESP_OPEN_RTOS) && !defined(__MBED__)
#pragma warning(disable : 4996)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Add CPU hardware definitions

#define _AVR_IO_H_
#include "noduino_F.h"
#define PSTR(str) (str)
#define _SFR_IO8(io_addr) ((io_addr))
//#include "C:\Program Files (x86)\Arduino\hardware\tools\avr\avr\include\avr\iomxx0_1.h"

typedef bool boolean;
//#define boolean bool
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

class __FlashStringHelper;
#define PGM_P  const char *
#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))


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
