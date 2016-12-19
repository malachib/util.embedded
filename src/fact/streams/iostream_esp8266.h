extern "C"
{
    #include "espressif/esp_common.h"
    #include "FreeRTOS.h"

    #include <unistd.h>
    #include <esp8266.h>
    #include <esp/uart.h>
    #include <stdio.h>
}

namespace FactUtilEmbedded { namespace std
{

class istream
{
public:
    istream& read(char* s, streamsize n)
    {
        //lwip_read(0, (void*)s, n);
        // TODO: switch away from posix mode one, name collision is just
        // waiting for us
        ::read(0, (void*)s, n);
        return *this;
    }

    int get()
    {
        //char ch;
        //fread(0, (void*)&ch, 1);

        //return -1;
        //return fgetc(stdin);
        // TODO: use more low-level version to avoid overhead & name collision
        return getchar();
    }
};

class ostream
{
public:
    ostream& write(char* s, streamsize n)
    {
        /*
         NOT working, so slumming it with a repeated put call
        ::write(0, (void*)s, n);
        */
        /*
        for(char ch : s)
        {

        }*/
        while(n--)
            put(*s++);
        return *this;
    }

    ostream& put(char ch)
    {
        putchar(ch);
        //write(&ch, 1);
        return *this;
    }
};

} }
