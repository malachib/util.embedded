#include "mbed.h"
#include "mbed_events.h"

#include <fact/iostream.h>

using namespace FactUtilEmbedded::std;

Serial pc(USBTX, USBRX);

namespace FactUtilEmbedded { namespace std {

#ifdef FEATURE_IOS_STREAMBUF_FULL
ostream cout;
#else
ostream cout(pc);
istream cin(pc);
#endif
ostream& clog = cout;
}}

int main()
{
    static int counter = 0;

    for(;;)
    {
        clog << "Waiting..." << counter++ << endl;
        printf("Waiting...%d\r\n", counter++);

        for(int i = 0; i < 100; i++)
        {
            if(cin.peek() != istream::traits_type::eof())
            {
                clog << "Got key: " << (char)cin.get() << endl;
            }
            wait(0.01);
        }
    }
    return 0;
}
