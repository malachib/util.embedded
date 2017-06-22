#include "catch.hpp"
#include "experimental/VFS.h"
#include <string.h>

using namespace FactUtilEmbedded::experimental::layer5;

SCENARIO( "Virtual File System tests", "[vfs]" )
{
    GIVEN("Test 1")
    {
        MemoryVFS<> vfs;

        vfs.open("test");
    }
}