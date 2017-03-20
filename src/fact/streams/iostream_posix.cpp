#ifdef __POSIX__

#include "../iostream.h"

namespace FactUtilEmbedded { namespace std { namespace experimental {

template<>
streamsize basic_streambuf_embedded<char, TEST_STREAM_T>::
    xsputn(const char* s, streamsize count)
{
    return fwrite(s, sizeof(char), count, &this->stream);
}


template<>
streamsize basic_streambuf_embedded<char, TEST_STREAM_T>::
    xsgetn(char* s, streamsize count)
{
    return fread(s, sizeof(char), count, &this->stream);
}

template<>
int_type basic_streambuf_embedded<char, TEST_STREAM_T>::
    sputc(char ch)
{
    return fputc(ch, &this->stream);
}


template<>
int_type basic_streambuf_embedded<char, TEST_STREAM_T>::
    sbumpc()
{
    return fgetc(&this->stream);
}

} } }

#endif

