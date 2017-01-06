#pragma once

extern "C"
{
    #include <unistd.h>
    #include <stdio.h>
}


// TODO: try using these once compile errors settle down
// gold: https://github.com/SuperHouse/esp-open-rtos/blob/8368929a6658082ebe5eee6926232986e7895f60/libc/xtensa-lx106-elf/include/reent.h
namespace FactUtilEmbedded { namespace std
{

class istream : public basic_istream<char>
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
        //char ch;
        //_read_r(??REENT??, 0, (void*)&ch, 1);
    }

    bool eof() { return false; }
};


class ostream : public basic_ostream<char>
{
public:
    __ostream_type& write(const char* s, streamsize n) override
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

    __ostream_type& put(char ch) override
    {
        putchar(ch);
        //write(&ch, 1);
        return *this;
    }
};

} }
