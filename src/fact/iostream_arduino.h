#pragma once

// rename, iostream_arduino no longer right name
// this sets up legacy fact_?streams based on whether we are using "real"
// or our shim iostream, but we always now will be using our ostream
#include "features.h"
#include "c_types.h"

#ifndef FEATURE_IOSTREAM
#include "iostream.h"
#ifndef ARDUINO
// Needed for __FlashStringHelper*
#include "noduino.h"
#endif

// TODO: put fact_ostream in the proper namespace - it's in NONE right now!
typedef FactUtilEmbedded::std::ostream fact_ostream;
typedef FactUtilEmbedded::std::istream fact_istream;

inline FactUtilEmbedded::std::basic_ostream<char>& operator <<(
    FactUtilEmbedded::std::basic_ostream<char>& out, const __FlashStringHelper* arg)
{ out << (const char*)(arg); return out; }

//#elif defined(FEATURE_IOSTREAM)
#else
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
