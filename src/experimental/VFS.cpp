#include "VFS.h"

namespace FactUtilEmbedded { namespace experimental { namespace layer5 {

template <>
FactUtilEmbedded::std::basic_iostream<char>&
    MemoryVFS < const char*>::open(const char* name, ios_base::openmode mode)
{

}

}}}