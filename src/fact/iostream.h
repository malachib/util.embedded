#pragma once

#ifndef FEATURE_IOSTREAM_SHIM
#error "Only include this for iostream shim compatibility"
#endif

// Compatibility shim for targets (which seem to be many) who don't have an iostream
// implementation.  Also can and should serve as a wrapper class around Stream implementations
// found in libs like Arduino & mbed OS

extern "C"
{
  #include <stdint.h>
  #include <stdlib.h>
  #include <string.h>
}

namespace FactUtilEmbedded { namespace std
{

typedef uint16_t streamoff;
typedef uint16_t streamsize;

//typedef basic_ostream __ostream_type;

template<class TChar>
class basic_istream
{
public:
    virtual bool eof() = 0;
};

template<class TChar>
class basic_ostream
{
public:
    typedef basic_ostream<TChar> __ostream_type;

    virtual __ostream_type& write(const TChar* s, streamsize n) = 0;
    virtual __ostream_type& put(TChar ch);

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
};

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
    __itoa(value, buffer, 10);
#else
    itoa(value, buffer, 10);
#endif

    return out << buffer;
}


inline basic_ostream<char>& endl(basic_ostream<char>& __os)
{ return __os.put('\n'); }


} }

#ifdef ESP_OPEN_RTOS
#include "streams/iostream_esp8266.h"
#else
#warning "Unknown architecture"
#endif

namespace FactUtilEmbedded { namespace std
{

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


} }
