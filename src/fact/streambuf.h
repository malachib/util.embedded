#ifndef UTIL_EMBEDDED_STREAMBUF_H
#define UTIL_EMBEDDED_STREAMBUF_H

#ifdef ESP_OPEN_RTOS
#elif defined(__MBED__)
#include <drivers/Stream.h>

//#include "mbed.h"
// FIX: We need this, but right now causes many compilation errors, looks like a collision
// on the F() macro
//#include <drivers/Serial.h>
#elif defined(ARDUINO)
#include <Arduino.h>
#endif


extern "C"
{
#include <stdio.h> // For POSIX modes
#include <stdint.h>
}

namespace FactUtilEmbedded { namespace std {


// TODO: find a better home for char_traits and friends.  Normally it lives in <string> -
// it seems likely most platforms would have this already, so check into that and if so, eliminate
// our special version

typedef uint16_t streamoff;
typedef uint16_t streamsize;

//  As per http://tuttlem.github.io/2014/08/18/getting-istream-to-work-off-a-byte-array.html
//  I won't try to stuff in a uint8_t as a TChar anywhere, although it seems like I could
// safely make a fully uint8_t version of things

template <class TChar> struct char_traits;
template <> struct char_traits<char>
{
    typedef char char_type;
    typedef int int_type;

    static int_type to_int_type(char ch) { return ch; }
    static int_type eof() { return -1; }
    static bool eq(char c1, char c2) { return c1 == c2; }
};



namespace experimental {

class Empty
{
};

template<class TChar, class TStream, class TBase=Empty, class Traits = char_traits <TChar>>
class basic_streambuf_embedded : public TBase
{
protected:
    typedef TChar char_type;
    typedef typename Traits::int_type int_type;
    TStream &stream;

    streamsize xsputn(const char_type *s, streamsize count);

    streamsize xsgetn(char_type *s, streamsize count);

public:
    typedef TStream stream_t;

    basic_streambuf_embedded(TStream &stream) : stream(stream)
    {}

    // http://putka.upm.si/langref/cplusplus.com/reference/iostream/streambuf/sgetn/index.html
    // acts like many sbumpc calls
    streamsize sgetn(char_type *s, streamsize count)
    {
        return xsgetn(s, count);
    }

    streamsize sputn(const char_type *s, streamsize count)
    {
        return xsputn(s, count);
    }


    int_type sputc(char_type ch)
    {
        bool success = xsputn(&ch, sizeof(ch)) == sizeof(ch);
        return success ? ch : Traits::eof();
    }


    // TODO: *possibly* implement underflow, if I like it...
    // Don't think I made this one quite right...
    int_type sbumpc()
    {
        char_type ch;

        bool success = xsgetn(&ch, sizeof(ch)) == sizeof(ch);

        return success ? Traits::to_int_type(ch) : Traits::eof();
    }

    int_type sgetc();

    // NOTE: this deviates from spec in that it won't wait for CR, for example,
    // to reflect characters are available
    streamsize in_avail();
};
}

#ifdef FEATURE_IOS_STREAMBUF_FULL
template<class TChar, class Traits = char_traits<TChar>>
class basic_streambuf
{
protected:
    typedef TChar char_type;
    typedef typename Traits::int_type int_type;

    virtual streamsize xsputn(const char_type* s, streamsize count) = 0;
    virtual streamsize xsgetn(char_type* s, streamsize count) = 0;

    //virtual int_type uflow() = 0;

public:
    streamsize sputn(const char_type* s, streamsize count)
    {
        return xsputn(s, count);
    }

    int_type sputc(char_type ch)
    {
        bool success = xsputn(&ch, sizeof(ch)) == sizeof(ch);
        return success ? (int_type) ch : Traits::eof();
    }

    streamsize sgetn(char_type* s, streamsize count)
    {
        return xsgetn(s, count);
    }

    // TODO: *possibly* implement underflow, if I like it...
    // Don't think I made this one quite right...
    int_type sbumpc()
    {
        char_type ch;

        bool success = xsgetn(&ch, sizeof(ch)) == sizeof(ch);

        return success ? Traits::to_int_type(ch) : Traits::eof();
    }
    
    // FIX: make this pure and implement in derived class
    virtual streamsize in_avail() { return 0; }
};
#else
#if defined(__MBED__)
#include "streams/iostream_mbed_streambuf.h"
#elif defined(__POSIX__)
#include "streams/iostream_posix_streambuf.h"
#elif defined(ARDUINO)
#include "streams/iostream_arduino_streambuf.h"
#else
#error "Architecture not yet supported"
#endif
#endif

} }

#endif //UTIL_EMBEDDED_STREAMBUF_H
