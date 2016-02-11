#pragma once

// If not specified, optimize for both code and data
#ifdef MEMORY_OPT
#define MEMORY_OPT_CODE   // Optimize down code SIZE
#define MEMORY_OPT_DATA   // Optimize down data SIZE
#endif

#ifndef MEMORY_OPT_CODE
#define CONSOLE_FEATURE_FULL
#define SERVICE_FEATURE_FULL
#endif

#ifdef CONSOLE_FEATURE_FULL
#define CONSOLE_FEATURE_AUTOCOMPLETE
#define CONSOLE_FEATURE_ENHANCED_CHARPROCESSOR
#endif

#ifdef SERVICE_FEATURE_FULL
#define SERVICE_FEATURE_EVENTS
#endif
//#define CONSOLE_FEATURE_MULTICONSOLE
//#define CONSOLE_BEHAVIOR_PROPAGATE
