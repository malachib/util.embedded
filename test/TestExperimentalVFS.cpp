#include "catch.hpp"
#include "experimental/VFS.h"
#include "experimental/Tree.h"
#include "fact/buffer.h"
#include <string.h>

using namespace FactUtilEmbedded::experimental::layer5;
namespace util = FactUtilEmbedded;


MemoryVFS<> _vfs1, _vfs2;
MemoryVFS<> _vfs3, _vfs4;
int location;


static void responder(uint16_t id, uint16_t parent_id)
{
    printf("Digging: current = %u, parent = %u\r\n", id, parent_id);
}

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

        tree.walk<nullptr, responder>(1);
    }
    GIVEN("Node from pointers")
    {
        namespace exp = FactUtilEmbedded::experimental::layer1;

        /*
        exp::Tree<
                //exp::GenericNode<VFS*, &vfs1, nullptr>,
                //exp::GenericNode<VFS*, &vfs2, &vfs1>,
                //exp::GenericNode<VFS*, &vfs3, &vfs1>,
                exp::GenericNode<VFS<>*, (VFS<>*)(&_vfs4), (VFS<>*)(&_vfs3)>>
                tree; */
    }
    GIVEN("Node from pointers2")
    {
        namespace exp = FactUtilEmbedded::experimental::layer1;
        //constexpr void* ptr1 = &_vfs3;
        //constexpr const void* ptr1 = "hello";
        constexpr void* ptr1 = nullptr;
        typedef exp::GenericNode2<nullptr, ptr1> test1;
        // tons of compile errors... why ?
        /*
        //constexpr void* ptr1 = &_vfs3;
        constexpr VFS<>& ptr1 = _vfs3;
        constexpr VFS<>* ptr2 = &_vfs4;

        exp::Tree<
                //exp::GenericNode<VFS*, &vfs1, nullptr>,
                //exp::GenericNode<VFS*, &vfs2, &vfs1>,
                //exp::GenericNode<VFS*, &vfs3, &vfs1>,
                exp::GenericNode2<&ptr1, ptr2>>
                tree; */
    }
}