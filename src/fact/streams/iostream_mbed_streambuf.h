#ifndef UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_MBED_H_H
#define UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_MBED_H_H

// NOT TESTED, likely will break compilation...
// for use with non-FEATURE_IOS_STREAMBUF_FULL mode only
// should never be manually included, only auto-included from ../iostream.h
// TODO: change this to use FileHandle once more tested

template<class TChar, class Traits = char_traits <TChar>>
class basic_streambuf : experimental::basic_streambuf_embedded<TChar, mbed::FileLike, Traits>
{
protected:
    typedef experimental::basic_streambuf_embedded<TChar, mbed::FileLike, Traits> base_t;
    //typedef char char_type;

public:
    basic_streambuf(FileLike& stream) : base_t(stream) {}

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

    int_type sputc(char_type ch)
    {
        bool success = xsputn(&ch, sizeof(ch)) == sizeof(ch);
        return success ? (int_type) ch : Traits::eof();
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
        char_type ch;

        bool success = xsgetn(&ch, sizeof(ch)) == sizeof(ch);

        return success ? Traits::to_int_type(ch) : Traits::eof();
    }
};

}


#endif //UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_MBED_H_H
