#pragma once

#include "features.h"
#include "c_types.h"

#ifdef FEATURE_IOSTREAM_SHIM
#include "iostream.h"

// TODO: put fact_ostream in the proper namespace - it's in NONE right now!
typedef FactUtilEmbedded::std::ostream fact_ostream;
typedef FactUtilEmbedded::std::istream fact_istream;

inline FactUtilEmbedded::std::basic_ostream<char>& operator <<(
    FactUtilEmbedded::std::basic_ostream<char>& out, const __FlashStringHelper* arg)
{ out << (const char*)(arg); return out; }

#elif defined(FEATURE_IOSTREAM)
#include <iostream>
// Needed for __FlashStringHelper*
#include "noduino.h"

typedef std::ostream fact_ostream;
typedef std::istream fact_istream;

// I feel like this is a pretty lousy solution, I want to keep the 'const'
// but nothing else seems to work
inline ::std::ostream& operator <<(::std::ostream& out, const __FlashStringHelper* arg)
{ out << (const char*)(arg); return out; }

#endif
