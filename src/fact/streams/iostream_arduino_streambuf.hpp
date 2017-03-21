// this gets included by either:
//  iostream_arduino_streambuf.h or
//  iostream_arduino_cpp
namespace experimental {

// Making this inline or .cpp defined seems to have zero effect on
// code size

//#define _INLINE inline
#define _INLINE
template<>
_INLINE streamsize basic_streambuf_embedded<char, Stream>::
    xsputn(const char* s, streamsize count)
{
    return this->stream.write(s, count);
}


template<>
_INLINE streamsize basic_streambuf_embedded<char, Stream>::
    xsgetn(char* s, streamsize count)
{
    return this->stream.readBytes(s, count);
}

template<>
_INLINE int basic_streambuf_embedded<char, Stream>::sputc(char ch)
{
    return this->stream.print(ch);
}


template<>
_INLINE int basic_streambuf_embedded<char, Stream>::sbumpc()
{
    return this->stream.read();
}

template<>
_INLINE int basic_streambuf_embedded<char, Stream>::sgetc()
{
    return this->stream.peek();
}

template<>
_INLINE streamsize basic_streambuf_embedded<char, Stream>::in_avail()
{
    return this->stream.available();
}

}
