#include "mbed.h"
#include <fact/iostream>

int main()
{
    static int counter = 0;

    for(;;)
    {
        printf("Waiting...%d\r\n", counter++);
        wait(1.0);
    }
    return 0;
}
