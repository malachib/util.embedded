//
// Created by malachi on 3/19/17.
//

#ifndef UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_STREAMBUF_H
#define UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_STREAMBUF_H

// specifically for non-FEATURE_IOS_STREAMBUF_FULL mode

//namespace FactUtilEmbedded { namespace std {

typedef ::FILE TEST_STREAM_T;
//typedef ::_IO_FILE TEST_STREAM_T;


template<class TChar, class Traits = char_traits <TChar>>
class basic_streambuf : public experimental::basic_streambuf_embedded<TChar, TEST_STREAM_T, Traits>
{
protected:
    typedef experimental::basic_streambuf_embedded<TChar, TEST_STREAM_T, Traits> base_t;

public:
    typedef TChar char_type;
    typedef TEST_STREAM_T stream_t;

    basic_streambuf(TEST_STREAM_T &stream) : base_t(stream)
    {}

    streamsize xsputn(const char_type *s, streamsize count)
    {
        return fwrite(s, sizeof(TChar), count, &this->stream);
    }

    streamsize xsgetn(char_type *s, streamsize count)
    {
        return fread(s, sizeof(TChar), count, &this->stream);
    }


    int_type sputc(char_type ch)
    {
        return fputc(ch, &this->stream);
    }

    int_type sbumpc()
    {
        return fgetc(&this->stream);
    }
};

//} }

#endif //UTIL_EMBEDDED_TESTS_IOSTREAM_POSIX_STREAMBUF_H
