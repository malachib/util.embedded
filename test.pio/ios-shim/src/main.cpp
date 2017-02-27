#include "mbed.h"
#include <fact/iostream>

using namespace FactUtilEmbedded::std;

Serial pc(USBTX, USBRX);

namespace FactUtilEmbedded { namespace std {

ostream cout(pc);
ostream& clog = cout;

}}

inline basic_ostream<char>& eol(basic_ostream<char>& __os)
{ return __os.write("\r\n", 2); }

int main()
{
    static int counter = 0;

    for(;;)
    {
        clog << "Waiting..." << counter++ << eol;
        printf("Waiting...%d\r\n", counter++);
        wait(1.0);
    }
    return 0;
}
