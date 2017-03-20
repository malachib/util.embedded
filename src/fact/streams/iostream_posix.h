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

typedef basic_istream<char> istream;


class ostream : public basic_ostream<char>
{
    typedef basic_ostream<char> base_t;
    //typedef typename base_t::basic_streambuf_t basic_streambuf_t;
    //typedef basic_streambuf_t::stream_t stream_t;

public:
#ifndef FEATURE_IOS_STREAMBUF_FULL
    ostream(stream_t& file) : base_t(file) {}
#endif

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
