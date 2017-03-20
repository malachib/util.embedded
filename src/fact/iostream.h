#pragma once

#ifdef ESP_OPEN_RTOS
#else
// ESP_OPEN_RTOS has some non-sprintf ways to convert numeric to strings
#define USING_SPRINTF
#endif

#ifdef ESP_OPEN_RTOS
#elif defined(__MBED__)
#include "Stream.h"
#endif

//#define FEATURE_IOS_STREAMBUF_FULL
#define FEATURE_IOS_GCOUNT
//#define FEATURE_IOS_IOSTREAM

// Compatibility shim for targets (which seem to be many) who don't have an iostream
// implementation.  Also can and should serve as a wrapper class around Stream implementations
// found in libs like Arduino & mbed OS

extern "C"
{
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(USING_SPRINTF) || defined(__POSIX__)
#include <stdio.h>
#include <inttypes.h>
#endif

}

namespace FactUtilEmbedded { namespace std
{
typedef uint16_t streamoff;
typedef uint16_t streamsize;

// TODO: ensure we're creating int_type properly
typedef int16_t int_type;

//  As per http://tuttlem.github.io/2014/08/18/getting-istream-to-work-off-a-byte-array.html
//  I won't try to stuff in a uint8_t as a TChar anywhere, although it seems like I could
// safely make a fully uint8_t version of things

template <class TChar> struct char_traits;
template <> struct char_traits<char>
{
    typedef char char_type;

    static int_type to_int_type(char ch) { return ch; }
    static int_type eof() { return -1; }
    static bool eq(char c1, char c2) { return c1 == c2; }
};


template<class TChar, class Traits = char_traits<TChar>>
class basic_streambuf_base
{
protected:
    typedef TChar char_type;
};

namespace experimental 
{
template<class TChar, class TStream, class Traits = char_traits<TChar>>
class basic_streambuf_embedded : public basic_streambuf_base<TChar, Traits>
{
protected:
    typedef TChar char_type;
    TStream& stream;
    
    streamsize xsputn(const char_type* s, streamsize count);
    streamsize xsgetn(char_type* s, streamsize count);
    
public:
    typedef TStream stream_t;
    
    basic_streambuf_embedded(TStream& stream) : stream(stream) {}

    // TODO: *possibly* implement underflow, if I like it...
    // Don't think I made this one quite right...
    int_type sbumpc()
    {
        char_type ch;

        bool success = xsgetn(&ch, sizeof(ch)) == sizeof(ch);

        return success ? Traits::to_int_type(ch) : Traits::eof();
    }
};
}

#ifdef FEATURE_IOS_STREAMBUF_FULL
template<class TChar, class Traits = char_traits<TChar>>
class basic_streambuf
{
protected:
    typedef TChar char_type;

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
};
#else
#if defined(__MBED__)
// FIX: Can't do this just yet, because this counts (apparently) as
// a specialization of basic_streambuf_embedded and collides with iostream_mbed version
// *or* perhaps this counts as an instantiation of the particular specialization 
// but the particular specialization itself has yet to be defined 
template<class TChar, class Traits = char_traits<TChar>>
class basic_streambuf : 
    public experimental::basic_streambuf_embedded<TChar, mbed::FileLike, Traits>
{
    typedef experimental::basic_streambuf_embedded<TChar, mbed::FileLike, Traits> base_t;
public:
    typedef mbed::FileLike stream_t;
    //typedef typename base_t::stream_t stream_t;
    
    basic_streambuf(stream_t& stream) : base_t(stream) {}
};
#include "streams/iostream_mbed_streambuf.h"
#elif defined(__POSIX__)
#include "streams/iostream_posix_streambuf.h"
/*
template<class TChar, class Traits = char_traits<TChar>>
class basic_streambuf :
    public experimental::basic_streambuf_embedded<TChar, ::_IO_FILE, Traits>
{
    typedef experimental::basic_streambuf_embedded<TChar, ::_IO_FILE, Traits> base_t;
public:
    typedef ::_IO_FILE stream_t;
    //typedef typename base_t::stream_t stream_t;

    basic_streambuf(stream_t& stream) : base_t(stream) {}
}; */
#else
#error "FEATURE_IOS_STREAMBUF_FULL required for this architecture"
#endif
#endif

//typedef basic_ostream __ostream_type;

class ios_base
{
public:
    typedef uint8_t fmtflags;

    static constexpr fmtflags dec = 1;
    static constexpr fmtflags hex = 2;
    static constexpr fmtflags basefield = dec | hex;
    
    typedef uint8_t openmode;
    
    static constexpr openmode app = 0x01;
    static constexpr openmode binary = 0x02;
    static constexpr openmode in = 0x04;
    static constexpr openmode out = 0x08;

    typedef uint8_t iostate;

    static constexpr iostate goodbit = 0x00;
    static constexpr iostate badbit = 0x01;
    static constexpr iostate failbit = 0x02;
    static constexpr iostate eofbit = 0x04;

private:
    fmtflags fmtfl;
    iostate _iostate;

protected:
    static constexpr openmode _openmode_null = 0; // proprietary, default of 'text'

public:
    fmtflags flags() const { return fmtfl; }
    fmtflags flags(fmtflags fmtfl)
    { return this->fmtfl = fmtfl; }

    iostate rdstate() const { return _iostate; }
    void clear(iostate state = goodbit) { _iostate = state; }
    void setstate(iostate state)
    {
        _iostate |= state;
    }

    bool good() const { return rdstate() == goodbit; }
    bool bad() const { return rdstate() & badbit; }
    bool fail() const { return rdstate() & failbit || rdstate() & badbit; }
    bool eof() const { return rdstate() & eofbit; }
};


template<class TChar, class Traits = char_traits<TChar>>
class basic_ios : public ios_base
{
public:
    typedef basic_streambuf<TChar, Traits> basic_streambuf_t;

protected:
#ifdef FEATURE_IOS_STREAMBUF_FULL
    basic_streambuf_t* _rdbuf;
    
    basic_streambuf_t* rdbuf() const { return _rdbuf; }
#else
    basic_streambuf_t _rdbuf;

    basic_streambuf_t* rdbuf() const { return (basic_streambuf_t*) &_rdbuf; }
    
    typedef typename basic_streambuf_t::stream_t stream_t;

    basic_ios(stream_t& stream) : _rdbuf(stream) {}
#endif
};


typedef basic_ios<char> ios;


template<class TChar, class traits = char_traits<TChar>>
class basic_istream :
#ifdef FEATURE_IOS_STREAMBUF_FULL
        virtual
#endif
        public basic_ios<TChar, traits>
{
    typedef basic_ios<TChar> base_t;
    typedef TChar char_type;
    typedef typename base_t::basic_streambuf_t basic_streambuf_t;

#ifdef FEATURE_IOS_GCOUNT
    streamsize _gcount = 0;

public:
    streamsize gcount() const { return _gcount; }
#endif

public:
    typedef basic_istream<TChar> __istream_type;

    int get()
    {
        return this->rdbuf()->sbumpc();
    }

    __istream_type& read(TChar* s, streamsize n)
    {
        // TODO: optimization point.  We want to do something
        // so that we don't inline this (and other read/write operations like it)
        // all over the place
        if(this->rdbuf()->sgetn(s, n) != n)
            setstate(base_t::eofbit);

        return *this;
    }

    // TODO: optimize, ensure this isn't inlined
    __istream_type& getline(char_type* s, streamsize count, char_type delim = '\n')
    {
        basic_streambuf_t* stream = this->rdbuf();

#ifdef FEATURE_IOS_GCOUNT
        _gcount = 0;
#endif

        for(;;)
        {
            int_type c = stream->sbumpc();

            if(traits::eq(c, traits::eof()))
            {
                this->setstate(base_t::eofbit);
                break;
            }

            if(!count-- || traits::eq(c, delim))
            {
                this->setstate(base_t::failbit);
                break;
            }

            *s++ = c;
#ifdef FEATURE_IOS_GCOUNT
            _gcount++;
#endif
        }

        *s = 0;

        return *this;
    }

#ifndef FEATURE_IOS_STREAMBUF_FULL
    typedef typename base_t::stream_t stream_t;

    basic_istream(stream_t& stream) : base_t(stream) {}
#endif
};

template<class TChar, class traits = char_traits<TChar>>
class basic_ostream :
#ifdef FEATURE_IOS_STREAMBUF_FULL
        virtual
#endif
        public basic_ios<TChar, traits>
{
    typedef basic_ios<TChar> base_t;
    
public:
    typedef basic_ostream<TChar> __ostream_type;

    virtual __ostream_type& write(const TChar* s, streamsize n) = 0;
    virtual __ostream_type& put(TChar ch) = 0;

    // When the time comes, these will replace the old virtual ones
    __ostream_type& write_new(const TChar* s, streamsize n)
    {
        this->rdbuf()->sputn(s, n);
        return *this;
    }


    __ostream_type& put_new(TChar ch)
    {
        if(this->rdbuf()->sputc(ch) == char_traits<TChar>::eof())
            setstate(base_t::eofbit);

        return *this;
    }

    //friend basic_ostream& operator<<(basic_ostream& (*__pf)(basic_ostream&));

    __ostream_type& operator<<(__ostream_type& (*__pf)(__ostream_type&))
    {
        return __pf(*this);
    }

    /*
    basic_ostream& operator<<(basic_ostream& (*__pf)(basic_ostream&))
    {
        return __pf(*this);
    }*/

#ifndef FEATURE_IOS_STREAMBUF_FULL
    typedef typename base_t::stream_t stream_t;

    basic_ostream(stream_t& stream) : base_t(stream) {}
#endif
};

#ifdef FEATURE_IOS_STREAMBUF_FULL
template <class TChar, class traits = char_traits<TChar>>
class basic_iostream :
        public basic_ostream<TChar, traits>,
        public basic_istream<TChar, traits>
{

};
#else
// don't use virtual inheritance and instead manually redefine basic_istream behaviors
// creates some visual code slop, but ends up compiling smaller (and maybe faster)
template <class TChar, class traits = char_traits<TChar>>
class basic_iostream :
        public basic_ostream<TChar, traits>
{

};
#endif



inline basic_ostream<char>& operator <<(basic_ostream<char>& out, const char* arg)
{
    return out.write(arg, strlen(arg));
}

inline basic_ostream<char>& operator <<(basic_ostream<char>& out, char ch)
{
    return out.put(ch);
}


inline basic_ostream<char>& operator <<(basic_ostream<char>& out, uint16_t value)
{
    char buffer[10];

#if ESP_OPEN_RTOS
    __utoa(value, buffer, 10);
#else
    snprintf(buffer, sizeof(buffer), "%u", value);
#endif

    return out << buffer;
}

inline basic_ostream<char>& operator<<(basic_ostream<char>& out, void* addr)
{
    char buffer[sizeof(uintptr_t) * 3];

#if ESP_OPEN_RTOS
    __utoa((uint32_t)addr, buffer, 16);
#else
    snprintf(buffer, sizeof(buffer), "%" PRIXPTR, (uintptr_t)addr);
#endif
    return out << buffer;
}


inline basic_ostream<char>& operator<<(basic_ostream<char>& out, int value)
{
    char buffer[10];

#if ESP_OPEN_RTOS
    __itoa(value, buffer, 10);
#else
    snprintf(buffer, sizeof(buffer), "%d", value);
#endif

    return out << buffer;
}


inline basic_ostream<char>& endl(basic_ostream<char>& __os)
{ return __os.put('\n'); }


inline basic_ostream<char>& dec(basic_ostream<char>& __os)
{
    __os.flags((__os.flags() & ~ios_base::basefield) | ios_base::dec);
    return __os;
}

inline basic_ostream<char>& hex(basic_ostream<char>& __os)
{
    __os.flags((__os.flags() & ~ios_base::basefield) | ios_base::hex);
    return __os;
}


// To change delimiters, we'll need to do something like this:
// http://stackoverflow.com/questions/7302996/changing-the-delimiter-for-cin-c
/*
inline basic_istream<char>& operator >>(basic_istream<char>& in, short& value)
{
    return *in;
}
*/

} }

#ifdef ESP_OPEN_RTOS
#include "streams/iostream_esp8266.h"
#elif defined(__POSIX__)
#include "streams/iostream_posix.h"
#elif defined(__MBED__)
#include "streams/iostream_mbed.h"
#elif defined(ARDUINO)
#include "streams/iostream_arduino.h"
#else
#warning "Unknown architecture"
#endif

namespace FactUtilEmbedded { namespace std
{
typedef basic_streambuf<char> streambuf;


// Shamelessly lifted from GCC4.8.5
/**
 *  @brief  Write a newline and flush the stream.
 *
 *  This manipulator is often mistakenly used when a simple newline is
 *  desired, leading to poor buffering performance.  See
 *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt11ch25s02.html
 *  for more on this subject.
*/
/*
typedef basic_ostream<_CharT, _Traits>            __ostream_type;

__ostream_type&
      operator<<(__ostream_type& (*__pf)(__ostream_type&))
      {
        // _GLIBCXX_RESOLVE_LIB_DEFECTS
        // DR 60. What is a formatted input function?
        // The inserters for manipulators are *not* formatted output functions.
        return __pf(*this);
      }

template<typename _CharT, typename _Traits>
  inline basic_ostream<_CharT, _Traits>&
  endl(basic_ostream<_CharT, _Traits>& __os)
  { return flush(__os.put(__os.widen('\n'))); }
  */

/*
__ostream_type& operator<<(__ostream_type& (*__pf)(__ostream_type&))
{
    return __pf(*this);
}*/

extern ostream cout;
extern istream cin;
extern ostream& clog;
extern ostream& cerr;

// FIX: Not a great location for this, but has to live somewhere for now
// based on C++ assert() macro specification, but a little wimpier
#ifndef ASSERT
#ifdef DEBUG
//#define ASSERT(condition) if(!(condition)) { cerr << "Failure: " << __FILE__ << __LINE__ << __func__ << endl; }
#define ASSERT(condition, message) \
{   \
    bool evaluated = condition; \
    if(!evaluated) { cerr << __func__ << ": " << message << endl; } \
}
#else
//#define ASSERT(condition)
#define ASSERT(condition, message)
#endif
#endif

} }
