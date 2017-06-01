#pragma once

extern "C" {
#include <stdint.h>
}

namespace FactUtilEmbedded { namespace std { namespace experimental {

struct locale
{

};

struct ctype_base
{
    typedef uint8_t mask;

    static constexpr mask space = 0x01;
};


template <class TChar>
class ctype : public ctype_base
{
public:
    bool is(mask m, TChar ch) const { return false; }
    const TChar* is(const TChar* low, const TChar* high, mask* vec) const { return nullptr; }
};

// specialization, deviating from standard in that locale is compile-time
// instead of runtime
template <>
class ctype<char>
{

};

template <class TFacet>
const TFacet& use_facet(const locale& loc);

template <class TChar>
inline bool isspace(TChar ch, const locale& loc)
{
    return use_facet<ctype<TChar>>(loc).is(ctype_base::space, ch);
}

}}}