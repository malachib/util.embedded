#include "catch.hpp"
#include "experimental/VFS.h"
#include "fact/buffer.h"
#include <string.h>

using namespace FactUtilEmbedded::experimental::layer5;
namespace util = FactUtilEmbedded;


SCENARIO( "Virtual File System tests", "[vfs]" )
{
    GIVEN("Test 1")
    {
        MemoryVFS<> vfs;

        vfs.open("test");
    }
    GIVEN("Aggregate")
    {
        MemoryVFS<> vfs1, vfs2;

        VFS<>* array[] = { &vfs1, &vfs2 };
        util::layer3::Array<VFS<>*> vfs(array, 2);

        AggregateVFS<const char*> _vfs(vfs);

        _vfs.exists("test");
    }
}