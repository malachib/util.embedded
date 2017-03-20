//
// Created by malachi on 3/19/17.
//

#ifndef UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_MBED_H_H
#define UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_MBED_H_H

// NOT TESTED, likely will break compilation...
// for use with non-FEATURE_IOS_STREAMBUF_FULL mode only
// should never be manually included, only auto-included from ../iostream.h
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
};
}


#endif //UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_MBED_H_H
