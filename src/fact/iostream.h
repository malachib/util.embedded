// Compatibility shim for targets (which seem to be many) who don't have an iostream
// implementation.  Also can and should serve as a wrapper class around Stream implementations
// found in libs like Arduino & mbed OS

extern "C"
{
  #include "espressif/esp_common.h"
  #include "FreeRTOS.h"

  #include <stdint.h>
  #include <sys/types.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <string.h>
  #include <esp8266.h>
  #include <esp/uart.h>
  #include <stdio.h>

  // For posix compatibility
  #include "fcntl.h"
  #include "unistd.h"
}

namespace FactUtilEmbedded { namespace std
{

typedef uint16_t streamoff;
typedef uint16_t streamsize;

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
