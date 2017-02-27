#include "mbed.h"
#include <fact/iostream>

using namespace FactUtilEmbedded::std;

Serial pc(USBTX, USBRX);

namespace FactUtilEmbedded { namespace std {

ostream cout(pc);
ostream& clog = cout;

}}

int main()
{
    static int counter = 0;

    for(;;)
    {
        clog << "Waiting..." << counter++ << endl;
        printf("Waiting...%d\r\n", counter++);
        wait(1.0);
    }
    return 0;
}
