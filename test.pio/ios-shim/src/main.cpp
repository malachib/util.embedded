#include "mbed.h"
#include <fact/iostream.h>

using namespace FactUtilEmbedded::std;

Serial pc(USBTX, USBRX);

namespace FactUtilEmbedded { namespace std {

#ifdef FEATURE_IOS_STREAMBUF_FULL
ostream cout;
#else
ostream cout(pc);
ostream& clog = cout;
#endif
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
