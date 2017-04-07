#include <Arduino.h>
#include <Console.h> // PROG: 302, DATA: 9 whether or not Console is included. Good

#define IOS_SIZE_TEST

namespace FactUtilEmbedded { namespace std {

#ifdef IOS_SIZE_TEST
ostream cout(Serial); // +8 bytes PROG, +4 bytes DATA
#endif

}}

using namespace FactUtilEmbedded::std;

void setup() 
{
#ifdef IOS_SIZE_TEST
    Serial.begin(115200);

    cout << F("Initializing") << endl; // ~80 bytes PROGMEM
    cout << F("Line 2") << endl; // +~60 bytes PROGMEM
    cout << F("Line 3") << 3 << endl; // +~200 bytes PROGMEM, 2 bytes DATA
#endif
}

void loop() {}
