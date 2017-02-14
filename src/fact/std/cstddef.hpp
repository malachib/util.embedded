#pragma once

#ifdef __MBED__
#include_next <cstddef>
#else
// shim library to simulate presence of C++ standard libs

namespace FactUtilEmbedded
{
namespace std
{

typedef ::size_t size_t;

}
}
#endif
