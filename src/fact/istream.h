#ifndef UTIL_EMBEDDED_ISTREAM_H
#define UTIL_EMBEDDED_ISTREAM_H

#include "streambuf.h"
#include "ios.h"
#include <cassert>

// FIX: Re-include this if
//  a) we're sure Arduino and other off-the-beaten-std path has it and
//  b) we feel like fighting with the standard std namespace (which algorithm seems to auto include
//     i.e. it seems to specify 'using namespace std'
//#include <algorithm> // for min function

#define FEATURE_IOS_GCOUNT

namespace FactUtilEmbedded { namespace std {

#ifdef min
#undef min
#endif

// lifted from http://www.cplusplus.com/reference/algorithm/min/
// FIX: put this into algorithm.h if we're gonna really roll with it
template <class T> const T& min (const T& a, const T& b)
{
    return !(b<a)?a:b;     // or: return !comp(b,a)?a:b; for version (2)
}

template<class TChar, class Traits = char_traits<TChar>>
class basic_istream :
#ifdef FEATURE_IOS_STREAMBUF_FULL
        virtual
#endif
        public basic_ios<TChar, Traits>
{
    typedef basic_ios<TChar> base_t;
    typedef TChar char_type;
    typedef typename base_t::basic_streambuf_t basic_streambuf_t;
    typedef typename Traits::int_type int_type;

    /**
     * It seems the proper behavior of 'peek' is ambiguous, so this helper
     * method *always* is nonblocking version of peek.
     * *may* have to change from sgetc() to a nonstandard speekc() not sure yet
     * @return
     */
    inline int_type real_peek()
    {
        // TODO: change this to call non-standard this->rdbuf()->speekc();
        return this->good() ? this->rdbuf()->sgetc() : Traits::eof();
    }

    /**
     *
     * @return true if data available, false if timeout occured
     */
    bool block_experimental()
    {
        // TODO: add timeout logic here
        while(!this->rdbuf()->in_avail());

        return true;
    }

#ifdef FEATURE_IOS_GCOUNT
    streamsize _gcount = 0;

public:
    streamsize gcount() const { return _gcount; }
#endif

public:
    typedef basic_istream<TChar, Traits> __istream_type;

    int get()
    {
        return this->rdbuf()->sbumpc();
    }


    // nonblocking read
    // UNTESTED
    streamsize readsome(TChar* s, streamsize count)
    {
        auto rdbuf = *(this->rdbuf());
        // if count > number of available bytes
        // then read only available bytes
        // otherwise read all of count
        streamsize m = min(count, rdbuf.in_avail());

        return rdbuf.sgetn(s, m);
    }

    __istream_type& read(TChar* s, streamsize n)
    {
        // TODO: optimization point.  We want to do something
        // so that we don't inline this (and other read/write operations like it)
        // all over the place
        if(this->rdbuf()->sgetn(s, n) != n)
            this->setstate(base_t::eofbit);

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

            if(Traits::eq(c, Traits::eof()))
            {
                this->setstate(base_t::eofbit);
                break;
            }

            if(!count-- || Traits::eq(c, delim))
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

#ifdef FEATURE_IOS_EXPERIMENTAL_INSPECT
    int_type inspect()
    {
        block_experimental();

        return real_peek();
    }
#endif

#ifdef FEATURE_IOS_EXPERIMENTAL_GETSOME
    int_type getsome()
    {
        return real_peek();
    }
#endif


    int_type peek()
    {
        // TODO: don't call real_peek() anymore here
        // put this line back:
        //    return this->good() ? this->rdbuf()->sgetc() : Traits::eof();
        return real_peek();
    }

    // delim test is disabled if delim is default value, which would be EOF
    basic_istream& ignore(streamsize count = 1)
    {
        while(count--) get();
        return *this;
    }

    // http://en.cppreference.com/w/cpp/io/basic_istream/ignore
    basic_istream& ignore(streamsize count, const int_type delim)
    {
        if(delim == Traits::eof()) return ignore(count);

        while(count--)
        {
            int_type ch = get();
            if(ch == delim) break;
        }
        return *this;
    }

#ifndef FEATURE_IOS_STREAMBUF_FULL
    typedef typename base_t::stream_t stream_t;

    //basic_istream(stream_t& stream) : base_t(stream) {}

    template <class _TStream, class ... TArgs>
    basic_istream(_TStream& stream, TArgs...args) : base_t(stream, args...) {}
#endif
};

typedef basic_istream<char> istream;


// To change delimiters, we'll need to do something like this:
// http://stackoverflow.com/questions/7302996/changing-the-delimiter-for-cin-c
/*
inline basic_istream<char>& operator >>(basic_istream<char>& in, short& value)
{
    return *in;
}
*/

#ifndef FEATURE_IOSTREAM

/**
 * Consume whitespace
 * @tparam TChar
 * @param __os
 * @return
 */
template <class TChar>
inline basic_istream<TChar>& ws(basic_istream<TChar>& __is)
{
    static_assert(&__is != nullptr, "Feature not yet ready");

    return __is;
    //isspace(' ');
}
#endif

} }
#endif //UTIL_EMBEDDED_ISTREAM_H
