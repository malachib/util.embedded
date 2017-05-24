#pragma once

#define FACT_UTIL_EMBEDDED_VERSION "0.8"

// If not specified, optimize for both code and data
#ifdef MEMORY_OPT
#define MEMORY_OPT_CODE   // Optimize down code SIZE
#define MEMORY_OPT_DATA   // Optimize down data SIZE
#endif

#if !defined(MEMORY_OPT_CODE)
#define CONSOLE_FEATURE_FULL
#if !defined(MEMORY_OPT_DATA) || MEMORY_OPT_DATA < 1
#define SERVICE_FEATURE_FULL
#else
#endif
#endif

#if defined(CONSOLE_FEATURE_FULL) && !defined(CONSOLE_FEATURE_FULL_SUPPRESS)
// autcomplete will autcomplete any of the immediate level of menu commands
#define CONSOLE_FEATURE_AUTOCOMPLETE
// autocomplete command autocompletes digging into nesting menus/commands also
#define CONSOLE_FEATURE_AUTOCOMPLETE_COMMAND
#endif

#if defined(CONSOLE_FEATURE_AUTOCOMPLETE_COMMAND) && !defined(CONSOLE_FEATURE_AUTOCOMPLETE)
#define CONSOLE_FEATURE_AUTOCOMPLETE
#endif

#if defined(CONSOLE_FEATURE_AUTOCOMPLETE) && !defined(CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR)
#define CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR
#endif

//#define CONSOLE_FEATURE_MULTICONSOLE
#ifdef CONSOLE_FEATURE_COUT
#define CONSOLE_BEHAVIOR_PROPAGATE
#endif

#if defined(SERVICE_FEATURE_FULL) && !defined(SERVICE_FEATURE_FULL_SUPPRESS)
#define SERVICE_FEATURE_EVENTS
#define SERVICE_FEATURE_RETAINED_STARTFUNC
#define SERVICE_FEATURE_RETAINED_DEPENDENCY
#endif

#ifndef DRIVER_FEATURE_VTABLE_SUPPRESS
#define DRIVER_FEATURE_VTABLE
#endif

#define VALIDATE_FEATURE_NULLCHECK

#define PROPERTY_FEATURE_VALIDATE

#define FACT_LIB_STRICT

#if defined(FEATURE_IOSTREAM_SHIM) || defined(FEATURE_IOSTREAM)
// TODO: rename this, or repair it, it is ambiguous with FEATURE_IOSTREAM
// in short, it exists to notify code to use std::cout - otherwise, a macro'd
// cout -> Serial might be in use
#define FEATURE_IOS
#endif

#ifdef FEATURE_IOSTREAM
#define FEATURE_STDLIBCPP
#endif

// Add 'inspect' method to istream for a blocking version of peek
// Not fully decided if we want a semi-non-standard non-blocking 'peek' with a non-standard blocking 'inspect'
// or
// a semi-standard blocking 'peek' (docs suggest it's NOT blocking, but implementations are)
// with a non-standard non-blocking 'anticipate' type of call
#define FEATURE_IOS_EXPERIMENTAL_INSPECT
//#define FEATURE_IOS_EXPERIMENTAL_ANTICIPATE

// Shouldn't hurt anything other than burning up a bit of memory as we test things
// this turns out a circular buffer for streambuf which ultimately will enable putback and unget
#ifdef FEATURE_IOS_EXPERIMENTAL_STREAMBUFBUF
#if ((FEATURE_IOS_EXPERIMENTAL_STREAMBUFBUF + 0) < 4)
#error "Must defined a value of 4 or higher if using FEATURE_IOS_EXPERIMENTAL_STREAMBUFBUF"
#endif
#endif