#pragma once

//#include "mbed.h"

extern "C"
{
    //#include <unistd.h>
    #include <stdio.h>
}

#include "Stream.h"

//#error "MBED OS not yet supported, but coming"


namespace FactUtilEmbedded { namespace std
{
using namespace mbed;

#ifdef FEATURE_IOS_STREAMBUF_FULL
class streambuf_mbed_stream : public streambuf
{
    FileLike& stream;
    
public:
    streambuf_mbed_stream(FileLike& stream) : stream(stream) {}

    virtual streamsize xsputn(const char_type* s, streamsize count)
    {
        stream.write(s, count);
        return count;
    }
    
    virtual streamsize xsgetn(char_type* s, streamsize count)
    {
        stream.read(s, count);
        return count;
    }
};
#endif

namespace experimental
{
template<>
class basic_streambuf_embedded<char, FileLike> : basic_streambuf_base<char>
{
protected:
    //typedef char char_type;
    FileLike& stream;

public:
    basic_streambuf_embedded(FileLike& stream) : stream(stream) {}

    streamsize xsputn(const char_type* s, streamsize count)
    {
        stream.write(s, count);
        return count;
    }
    
    streamsize xsgetn(char_type* s, streamsize count)
    {
        stream.read(s, count);
        return count;
    }
};
}

class istream : public basic_istream<char>
{
    Stream* input;
    FileLike& getFile() { return *input; }

public:
    istream& read(char* s, streamsize n)
    {
        FileLike& in = getFile();

        in.read(s, n);

        return *this;
    }

    int get()
    {
        return input->getc();
    }

    bool eof() { return false; }
};


class ostream : public basic_ostream<char>
{
    typedef basic_ostream<char> base_t;
    
    openmode _openmode;
    Stream& output;
    FileLike& getFile() const { return output; }

public:
    ostream(Stream& o, openmode _openmode = _openmode_null) : 
#ifndef FEATURE_IOS_STREAMBUF_FULL
        base_t(o),
#endif
        output(o)
    {
        this->_openmode = _openmode | out;
    }

    // TODO: make non-binary '\r' handler here also
    __ostream_type& write(const char* s, streamsize n) override
    {
        FileLike& file = getFile();

        file.write(s, n);

        return *this;
    }

    __ostream_type& put(char ch) override
    {
        if(((_openmode & binary) == 0) && ch == '\n')
            output.putc('\r');
            
        output.putc(ch);

        return *this;
    }

    /*
    __ostream_type& flush()
    {
        // I think the pio flavor of mbed doesn't have this yet
        getFile().sync();
        return *this;
    } */
};

} }
