#ifndef UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_MBED_H_H
#define UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_MBED_H_H

#ifdef FEATURE_IOS_STREAMBUF_FULL
#else
// TODO: change this to use FileHandle once more tested
// TODO: once F() collision is remedied, enable Serial code

class basic_streambuf_mbed
{
public:
    typedef uint8_t traits;

    static constexpr traits none = 0;
    static constexpr traits serialbit = 0x01;

    // use this to create an sgetc by cleverly using _in_avail and sbumpc
    static constexpr traits sbumpccachebit = 0x02;

    bool is_serial() { return _traits & serialbit; }
    bool is_sbumpc_cache() { return _traits & sbumpccachebit; }

    // Since mbed OS somewhat scatters these three, directly function pointer them
    // out.  Don't do virtual tables, since the number of basic_streambufs in a system
    // is gonna be low, so explicit function pointers almost definitely smaller and faster
    streamsize (*_in_avail)(void*);
    int (*_sbumpc)(void *);
    union
    {
        int (*_sgetc)(void*);
        char char_cache;
    };

protected:
    traits _traits = none;
};

template<class TChar, class Traits = char_traits<TChar>>
class basic_streambuf : public experimental::basic_streambuf_embedded<TChar, mbed::FileLike, basic_streambuf_mbed, Traits>
{
protected:
    typedef experimental::basic_streambuf_embedded<TChar, mbed::FileLike, basic_streambuf_mbed, Traits> base_t;
    typedef TChar char_type;
    typedef typename Traits::int_type int_type;

    mbed::FileLike& fileLike() { return base_t::stream; }

    streamsize xsputn(const char_type* s, streamsize count)
    {
        this->stream.write(s, count);
        return count;
    }

    streamsize xsgetn(char_type* s, streamsize count)
    {
        this->stream.read(s, count);
        return count;
    }

    static streamsize serial_in_avail(void* stream)
    {
        auto _stream = (mbed::Stream*) stream;
        auto serial = (mbed::Serial*) _stream;
        return serial->readable();
    }

public:
    // IMPORTANT: only Stream and Serial can get in, meaning that even though we track FileLike,
    // we actually are interacting with a Stream always.  We do this because Stream has protected
    // some methods which we need from FileLike (I can't explain why that decision was made)
    // - read
    // - write
    // yet made public others which we also need that FileLike *doesn't* have:
    // - getc
    // Not quite ready yet, something is stopping us from casting to Stream


    // FIX: fn pointer for sbumpc super hacky, instead fix mbed::Stream casting
    basic_streambuf(mbed::FileLike& stream,
                    streamsize (*_in_avail)(void*) = nullptr,
                    int (*_sbumpc)(void*) = nullptr,
                    int (*_sgetc)(void*) = nullptr) :
        base_t(stream)
    {
        this->_in_avail = _in_avail;
        this->_sbumpc = _sbumpc;
        this->_sgetc = _sgetc;
        if(_in_avail != nullptr && _sgetc == nullptr)
        {
            // Not activating feature just yet, want to check in working code
            this->_traits = basic_streambuf_mbed::sbumpccachebit;
            // already should be 0 from the _sgetc assignment
            //this->char_cache = 0;
        }
    }

    basic_streambuf(mbed::Serial& stream) : base_t(stream)
    {
        this->_traits = basic_streambuf_mbed::serialbit | basic_streambuf_mbed::sbumpccachebit;;
        this->_in_avail = serial_in_avail;
        this->char_cache = 0;
    }

    int_type sputc(char_type ch)
    {
        bool success = xsputn(&ch, sizeof(ch)) == sizeof(ch);
        return success ? ch : Traits::eof();
    }

    // http://putka.upm.si/langref/cplusplus.com/reference/iostream/streambuf/sgetn/index.html
    // acts like many sbumpc calls
    streamsize sgetn(char_type* s, streamsize count)
    {
        return xsgetn(s, count);
    }

    // TODO: *possibly* implement underflow, if I like it...
    // Don't think I made this one quite right...
    // TODO: optimize and reuse via specialization, if we can
    int_type sbumpc()
    {
        if(this->is_sbumpc_cache())
        {
            if(this->char_cache)
            {
                short temp = this->char_cache;
                this->char_cache = 0;
                return temp;
            }
        }
        if(this->_sbumpc != nullptr) return this->_sbumpc(&this->stream);
        /*
        auto _stream = (mbed::Stream*) &this->stream;

        return _stream->getc();*/

        char_type ch;

        bool success = xsgetn(&ch, sizeof(ch)) == sizeof(ch);

        return success ? Traits::to_int_type(ch) : Traits::eof();
    }

    streamsize sputn(const char_type* s, streamsize count)
    {
        return xsputn(s, count);
    }


    int_type sgetc()
    {
        // NOTE: should only be available when _sgetc is null
        if(this->is_sbumpc_cache())
        {
            if(this->char_cache)
            {
                short temp = this->char_cache;
                this->char_cache = 0;
                return temp;
            }
            else
            {
                // NOTE: _in_avail should ALWAYS be available when using is_sbumpc_cache
                if(this->_in_avail(&this->stream))
                {
                    return this->char_cache = sbumpc();
                }
            }
        }

        if(this->_sgetc != nullptr) return this->_sgetc(&this->stream);

        return Traits::eof();
    }

    streamsize in_avail()
    {
        if(this->_in_avail != nullptr) return this->_in_avail(&this->stream);

        return 0;
    }
};
#endif

#endif //UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_MBED_H_H
