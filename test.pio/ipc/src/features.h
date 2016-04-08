// All tests with ATMEGA32U4
//
// IPC for this example:
// uses 4650 - 4490 code so 160 bytes of code
//
// uses  298 -  164 data so 134 bytes of data
//    64 bytes for buffer: 70 bytes left
//    
#define FEATURE_IPC

// CONSOLE for this example:
// uses 7146 - 4650 code so 2496 bytes of code
// uses  491 - 298 data so 193 bytes of data
#define FEATURE_CONSOLE

// IPC2 + IPC + CONSOLE:
// uses 7392 - 7146 code so 246 bytes of code
// uses  509 -  491 data so 18 bytes of data
#define FEATURE_IPC2
