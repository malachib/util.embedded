#include "VFS.h"

namespace fstd = FactUtilEmbedded::std;

namespace FactUtilEmbedded { namespace experimental { namespace layer5 {

template <>
fstd::basic_iostream<char>&
    MemoryVFS < const char*>::open(const char* name, ios_base::openmode mode)
{

}

template <>
fstd::ostream&
MemoryVFS < const char*>::create(const char* name)
{

}


template <>
void MemoryVFS < const char*>::close(fstd::ios_base& stream)
{

}



template <>
bool MemoryVFS < const char*>::exists(const char*)
{
    return true;
}


}}}