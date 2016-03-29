#pragma once

#if defined(__AVR_ATmega32U4__)
// https://harizanov.com/2013/02/power-saving-techniques-on-the-atmega32u4/
#define AVR_USB
#define AVR_TIMER_COUNT 4
#define AVR_USART_COUNT 2
#endif

#if defined(__AVR_ATtiny85__)
// TODO: for Attiny, also do some prescalar goodies CLKPR = bit(CLKPCE) - but not in power area
#define AVR_USI
#define AVR_TIMER_COUNT 2
#endif

#if defined(__AVR_ATmega328P__)
#define AVR_TIMER_COUNT 2
#define AVR_PICOPOWER
#endif

#if not defined(__AVR_ATtiny85__)
#define AVR_TWI
#define AVR_SPI
#endif

#ifdef AVR_PICOPOWER
#define AVR_BOD
#endif


#ifdef AVR_BOD
#define PARAM_BOD_DISABLE_S  bool bod_disable = false
#define PARAM_BOD_DISABLE   ,bool bod_disable = false
#define PARAM_BOD_CALL_S     bod_disable
#define PARAM_BOD_CALL      ,bod_disable
#else
#define PARAM_BOD_DISABLE_S
#define PARAM_BOD_DISABLE
#define PARAM_BOD_CALL_S
#define PARAM_BOD_CALL
#endif
