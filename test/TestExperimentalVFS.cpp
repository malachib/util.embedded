#include "catch.hpp"
#include "experimental/VFS.h"
#include "experimental/Tree.h"
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
    GIVEN("Nodes")
    {
        namespace exp = FactUtilEmbedded::experimental::layer1;

        exp::Tree<
                exp::Node<1, 0>,
                exp::Node<2, 1>,
                exp::Node<3, 1>,
                exp::Node<4, 1>,
                exp::Node<5, 2>>
                tree;

        auto id = tree.get_parent(1);
        id = tree.get_parent(2);
        id = tree.get_parent(3);

        auto count = tree.child_count(1);

        id = tree.get_child(1, 0);
        id = tree.get_child(1, 1);

        count = tree.child_count(1);
    }
}