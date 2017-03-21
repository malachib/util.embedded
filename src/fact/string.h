#pragma once

#include "buffer.h"

namespace FactUtilEmbedded { namespace experimental {

namespace layer1 {

template <size_t size>
struct string : public ::FactUtilEmbedded::layer1::Array<char, size>
{
public:
    string(const char* source)
    {
        memccpy(this->getData(), source, size, size);
    }
};

template <typename T, size_t N>
constexpr size_t size(T (&)[N]) {
    return N;  // size of array
}

template <typename T, size_t size = sizeof(T)>
string<size> getString(T source)
{
    return string<size>(source);
}

template <typename T, size_t N>
string<N> getString2(T (&s)[N])
{
    return string<N> { s };
}

}

} }
