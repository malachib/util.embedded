// Bit by bit, phase out lib.h
#ifndef _mal_lib_h
#define _mal_lib_h

#include "debug.h"

// Mainly for WIN32
#ifdef _DEBUG
#define DEBUG 1
#endif

#include "c_types.h"


#include "../Handler.h"
#include "CircularBuffer.h"
#include "Profiler.h"
#include "observer.h"

namespace FactUtilEmbedded
{

struct IFuncProvider
{
  virtual int Func(int x) = 0;
};

struct TriangleWaveProvider : IFuncProvider
{
  int Func(int x)
  {
    return 0;
  }
};





// TODO: integrate this with PSTR_Property
#define PROPERTY_CLASS(className, type, name) \
class className                               \
{                                             \
  type value;                                 \
\
protected:                                    \
\
  className() {}                              \
  className(type value) : value(value) {}  \
\
  void set##name(type value) { this->value = value; } \
public:                                       \
  type get##name() const { return value; } \
};

PROPERTY_CLASS(Named, const __FlashStringHelper*, Name)
PROPERTY_CLASS(Described, const __FlashStringHelper*, Description)

}

namespace util = FactUtilEmbedded;

#endif
