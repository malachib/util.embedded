#include <Arduino.h>
#include <Console.h> // PROG: 302, DATA: 9 whether or not Console is included. Good

#define IOS_SIZE_TEST
#define IOS_SIZE_TEST2
#define IOS_SIZE_TEST3

namespace FactUtilEmbedded { namespace std {

#ifdef IOS_SIZE_TEST
ostream cout(Serial); // +8 bytes PROG, +4 bytes DATA
#endif

}}

using namespace FactUtilEmbedded::std;

void setup() 
{
    Serial.begin(115200);

#ifdef IOS_SIZE_TEST2
    cout << F("Initializing") << endl; // ~80 bytes PROGMEM
#ifdef IOS_SIZE_TEST3
    cout << F("Line 2") << endl; // +~60 bytes PROGMEM
    cout << F("Line 3") << 3 << endl; // +~200 bytes PROGMEM
#endif
#else
    // Something's odd, because my couts shouldn't be *smaller* but they are
    Serial.println(F("Initializing")); // ~84 bytes PROGMEM, 2 bytes DATA
    Serial.println(F("Line 2")); // +48 bytes PROGMEM
    Serial.print(F("Line 3")); // +34 bytes PROGMEM
    Serial.println(3); // +60 bytes PROGMEM
    
#endif
}

void loop() {}
