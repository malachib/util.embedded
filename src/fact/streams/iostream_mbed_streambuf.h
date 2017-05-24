#ifndef UTIL_EMBEDDED_STREAMBUF_MBED_H
#define UTIL_EMBEDDED_STREAMBUF_MBED_H

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

    /// @brief Are we operating in one-char cache mode
    bool is_sbumpc_cache() { return _traits & sbumpccachebit; }

    // Since mbed OS somewhat scatters these three, directly function pointer them
    // out.  Don't do virtual tables, since the number of basic_streambufs in a system
    // is gonna be low, so explicit function pointers almost definitely smaller and faster
    streamsize (*_in_avail)(void*);
    int (*_sbumpc)(void *);
    union
    {
        int (*_sgetc)(void*);
        int16_t char_cache;
    };

    // use this to determine of we have a character in char_cache or not
    // FIX: right now is a zero-compare, which obviously will work poorly for binary
    // so in a proper testable environment change to something else (like EOF/-1)
    bool is_char_cache_filled() { return char_cache; }

protected:
    basic_streambuf_mbed(streamsize (*_in_avail)(void*),
        int (*_sbumpc)(void*),
        int (*_sgetc)(void*))
    {
        this->_in_avail = _in_avail;
        this->_sbumpc = _sbumpc;
        this->_sgetc = _sgetc;

        if(_in_avail != nullptr && _sgetc == nullptr)
        {
            this->_traits = basic_streambuf_mbed::sbumpccachebit;
        }
        else
            this->_traits = basic_streambuf_mbed::none;
    }

    traits _traits = none;
};


namespace layer3 {

template<>
inline streamsize basic_streambuf<char, mbed::FileLike, char_traits<char>>::xsputn(const char* s, streamsize count)
{
    this->stream.write(s, count);
    return count;
}


template<>
inline streamsize basic_streambuf<char, mbed::FileLike, char_traits<char>>::xsgetn(char* s, streamsize count)
{
    this->stream.read(s, count);
    return count;
}

}

template<class TChar, class Traits = char_traits<TChar>>
class basic_streambuf : public layer3::basic_streambuf<TChar, mbed::FileLike, Traits>,
    public basic_streambuf_mbed
{
protected:
    typedef layer3::basic_streambuf<TChar, mbed::FileLike, Traits> base_t;
    typedef TChar char_type;
    typedef typename Traits::int_type int_type;

    mbed::FileLike& fileLike() { return base_t::stream; }

    static streamsize serial_in_avail(void* stream)
    {
        auto _stream = (mbed::Stream*) stream;
        auto serial = (mbed::Serial*) _stream;
        return serial->readable();
    }


    static int stream_sbumpc(void* stream)
    {
        auto _stream = (mbed::Stream*) stream;

        return _stream->getc();
    }


    // blocking function
    void wait_for_input()
    {
#ifdef FEATURE_IOS_TIMEOUT
#error "Need FAL timer functions before this feature can be used"
        //while(in_avail())
#else
        while(in_avail() == 0);
#endif
    }

    /**
     * this is a 100% non blocking version of sgetc
     * @return
     */
    int_type speekc(bool& have_data)
    {
#ifdef FEATURE_IOS_EXPERIMENTAL_TRAIT_NODATA
        static constexpr int_type result_nodata = Traits::nodata();
#else
        static constexpr int_type result_nodata = Traits::eof();
#endif
        /// mbed streams don't natively have a non-blocking get/peek mode
        /// so we have to workaround it by either precaching a byte, if necessary
        /// or by calling a specialized helper function ptr

        have_data = true;
        // NOTE: should only be available when _sgetc is null
        // indicates we are using a simplistic one-character buffer mechanism
        // to handle peek operations
        if(this->is_sbumpc_cache())
        {
            if(is_char_cache_filled())
            {
                // don't clear it out
                short temp = this->char_cache;
                return temp;
            }
            else
            {
                // NOTE: _in_avail should ALWAYS be available when using is_sbumpc_cache
                if(this->_in_avail(&this->stream))
                {
                    return this->char_cache = base_t::sbumpc();
                }

                have_data = false;
            }
        }

        // NOTE: might need to bring in a _speekc too?  at a minimum
        // if we move this function to speekc then we can't do this here
        //else if(this->_sgetc != nullptr) return this->_sgetc(&this->stream);

        return result_nodata;
    }


public:
    // Following represents ideal architecture.  Have not reached it just yet
    // IMPORTANT: only Stream and Serial can get in, meaning that even though we track FileLike,
    // we actually are interacting with a Stream always.  We do this because Stream has protected
    // some methods which we need from FileLike (I can't explain why that decision was made)
    // - read
    // - write
    // yet made public others which we also need that FileLike *doesn't* have:
    // - getc
    // Not quite ready yet, something is stopping us from casting to Stream


    // NOTE: this constructor largely untested
    // FIX: fn pointer for sbumpc super hacky, instead fix mbed::Stream casting
    // FIX: clean up inheritance/initialization of basic_streambuf_mbed
    basic_streambuf(mbed::Stream& stream,
                    streamsize (*_in_avail)(void*)) :
            base_t(stream),
            basic_streambuf_mbed(_in_avail, stream_sbumpc, nullptr)
    {
    }

    basic_streambuf(mbed::FileLike& stream,
                    streamsize (*_in_avail)(void*) = nullptr,
                    int (*_sbumpc)(void*) = nullptr,
                    int (*_sgetc)(void*) = nullptr) :
        base_t(stream),
        basic_streambuf_mbed(_in_avail, _sbumpc, _sgetc)
    {
    }

    basic_streambuf(mbed::Serial& stream) : base_t(stream),
                                            basic_streambuf_mbed(serial_in_avail, nullptr, nullptr)
    {
        this->_traits |= basic_streambuf_mbed::serialbit;
    }

    // TODO: *possibly* implement underflow, if I like it...
    // Don't think I made this one quite right...
    // TODO: optimize and reuse via specialization, if we can
    int_type sbumpc()
    {
        // NOTE: should only be available when _sgetc is null
        if(this->is_sbumpc_cache())
        {
            if(is_char_cache_filled())
            {
                short temp = this->char_cache;
                this->char_cache = 0;
                return temp;
            }
        }

        if(this->_sbumpc != nullptr) return this->_sbumpc(&this->stream);

        return base_t::sbumpc();
    }


    int_type sgetc()
    {
        /*
#ifdef FEATURE_IOS_EXPERIMENTAL_TRAIT_NODATA
        static constexpr int_type result_nodata = Traits::nodata();
#else
        static constexpr int_type result_nodata = Traits::eof();
#endif */

#ifndef FEATURE_IOS_EXPERIMENTAL_NONBLOCKING_PEEK
        wait_for_input();
#endif

        // NOTE: should only be available when _sgetc is null
        // indicates we are using a simplistic one-character buffer mechanism
        // to handle peek operations
        if(this->is_sbumpc_cache())
        {
            bool have_data;
            return speekc(have_data);
        }
        // NOTE: might need to bring in a _speekc too?  at a minimum
        // if we move this function to speekc then we can't do this here
        else if(this->_sgetc != nullptr) return this->_sgetc(&this->stream);

        return Traits::eof();
    }

#ifdef FEATURE_IOS_SPEEKC
    /**
     * Gaurunteed non-blocking glance into incoming buffer
     */
    int_type speekc()
    {
        bool have_data;
        return speekc(have_data);
    }
#endif

    streamsize in_avail()
    {
        if(this->_in_avail != nullptr) return this->_in_avail(&this->stream);

        return 0;
    }
};
#endif

#endif //UTIL_EMBEDDED_STREAMBUF_MBED_H
