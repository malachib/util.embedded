#ifndef UTIL_EMBEDDED_IOS_H
#define UTIL_EMBEDDED_IOS_H

#include "streambuf.h"

namespace FactUtilEmbedded { namespace std {

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
    fmtflags flags() const
    { return fmtfl; }

    fmtflags flags(fmtflags fmtfl)
    { return this->fmtfl = fmtfl; }

    iostate rdstate() const
    { return _iostate; }

    void clear(iostate state = goodbit)
    { _iostate = state; }

    void setstate(iostate state)
    {
        _iostate |= state;
    }

    bool good() const
    { return rdstate() == goodbit; }

    bool bad() const
    { return rdstate() & badbit; }

    bool fail() const
    { return rdstate() & failbit || rdstate() & badbit; }

    bool eof() const
    { return rdstate() & eofbit; }
};


template<class TChar, class Traits = char_traits <TChar>>
class basic_ios : public ios_base
{
public:
    typedef basic_streambuf <TChar, Traits> basic_streambuf_t;

protected:
#ifdef FEATURE_IOS_STREAMBUF_FULL
    basic_streambuf_t* _rdbuf;

public:
    basic_streambuf_t* rdbuf() const { return _rdbuf; }
#else
    basic_streambuf_t _rdbuf;

    typedef typename basic_streambuf_t::stream_t stream_t;

    basic_ios(stream_t &stream) : _rdbuf(stream)
    {}

public:
    basic_streambuf_t *rdbuf() const
    { return (basic_streambuf_t *) &_rdbuf; }

#endif
};


typedef basic_ios<char> ios;


} }

#endif //UTIL_EMBEDDED_IOS_H
