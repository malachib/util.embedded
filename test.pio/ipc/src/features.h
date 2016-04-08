// All tests with ATMEGA32U4
//
// IPC for this example:
// uses 4650 - 4490 code so 160 bytes of code
// uses  186 -  164 data so 24 bytes of data
#define FEATURE_IPC

// CONSOLE + IPC :
// uses 7146 - 4650 code so 2496 bytes of code
// uses  379 - 186 data so 193 bytes of data
//#define FEATURE_CONSOLE

// IPC2 + IPC + CONSOLE:
// uses 7392 - 7146 code so 246 bytes of code
// uses  397 -  379 data so 18 bytes of data
// IPC + IPC2:
// uses 4786 - 4650 code so 136 bytes of code
// uses  192 -  186 data so 6 bytes of data
#define FEATURE_IPC2

// All of above + IPC3:
// uses 7576 - 7392 code so 204 bytes of code
// uses  409 -  397 data so 8 bytes of data
// IPC + IPC2 + IPC3 (no console):
// uses 4860 - 4786 code so 74 bytes of code
// uses  192 -  192 data so 0 bytes of data
#define FEATURE_IPC3
