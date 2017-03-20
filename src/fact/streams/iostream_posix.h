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

namespace experimental
{
template<>
class basic_streambuf_embedded<char, _IO_FILE> : public basic_streambuf_base<char>
{
protected:
    //typedef char char_type;
    _IO_FILE& stream;

public:
    basic_streambuf_embedded(_IO_FILE& stream) : stream(stream) {}

    streamsize xsputn(const char_type* s, streamsize count)
    {
        return fwrite(s, sizeof(char), count, &stream);
    }

    streamsize xsgetn(char_type* s, streamsize count)
    {
        return fread(s, sizeof(char), count, &stream);
    }
};
}

class istream : public basic_istream<char>
{
public:
#ifndef FEATURE_IOS_STREAMBUF_FULL
    istream(_IO_FILE& file) : basic_istream<char>(file) {}
#endif

    bool eof() { return false; }
};


class ostream : public basic_ostream<char>
{
public:
#ifndef FEATURE_IOS_STREAMBUF_FULL
    ostream(_IO_FILE& file) : basic_ostream<char>(file) {}
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
