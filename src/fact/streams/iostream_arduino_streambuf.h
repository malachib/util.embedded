#pragma once

#ifdef FEATURE_IOS_STREAMBUF_FULL
#else

template <class TChar, class Traits = char_traits<TChar>>
using basic_streambuf = layer3::basic_streambuf<TChar, Stream, Traits>;

#ifdef FEATURE_IOS_STREAMBUF_INLINE
#include "iostream_arduino_streambuf.hpp"
#endif

#endif
