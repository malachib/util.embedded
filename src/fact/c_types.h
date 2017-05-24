// TODO: Clean this up, we shouldn't rely on c_types to also yank in noduino stuff
// probably bring back the <Arduino.h> includes for things that REALLY use those datatypes
// and make it map to nodoino.h when relevant

#pragma once

#ifndef ARDUINO
#include "noduino.h"
#else
#if (!CYPRESS_PSOC)
#ifndef Arduino_h
#include <Arduino.h>
#endif
// AVR toolchain already defines this
//#define AVR
#else
// Only for Cypress PSoC
#define NODUINO 1
#endif
#endif

#ifndef ARDUINO
#include <stdint.h>
#else
// MS unit tests define these for arm_neon
#if !defined(MS_CPP_UNITTESTFRAMEWORK) && !defined(ARDUINO) && !defined(_UINT32_T)
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
#endif
#endif


#ifndef byte
typedef unsigned char byte;
#endif

#include "features.h"

#ifndef FEATURE_STDLIBCPP
namespace FactUtilEmbedded { namespace std {

// TODO: move this into string.h
template <class TChar> struct char_traits;

template <class TChar = char, class TTraits = char_traits<TChar>>
typename TTraits::int_type isspace(typename TTraits::int_type ch)
{
    static constexpr typename TTraits::char_type WHITESPACE[] = { 0x20, 0x0C, 0x0A, 0x0D, 0x09, 0x0B };
    return TTraits::find(WHITESPACE, sizeof(WHITESPACE), ch);
};

}}
#endif