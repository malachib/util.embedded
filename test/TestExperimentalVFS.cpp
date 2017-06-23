#include "catch.hpp"
#include "experimental/VFS.h"
#include "experimental/Tree.h"
#include "fact/buffer.h"
#include <string.h>

using namespace FactUtilEmbedded::experimental::layer5;
namespace util = FactUtilEmbedded;

struct blah
{
    int blah;
};



template <class T>
struct blah_base
{

};

template <class T>
struct blah2 : public blah_base<T>
{
    virtual bool test() { return true; }
};

MemoryVFS<> _vfs1, _vfs2;
MemoryVFS<> _vfs3, _vfs4;

int location;
blah blah1;
blah2<int> blah3;

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

        int expected_results_id[] = { 1, 2, 5, 3, 4 };
        int expected_results_id_parent[] = { 0, 1, 2, 1, 1 };
        int counter = 0;

        tree.walk(1, [&](int id, int id_parent)
        {
            INFO("Processing count " << counter)

            REQUIRE(expected_results_id[counter] == id);
            REQUIRE(expected_results_id_parent[counter] == id_parent);

            counter++;
        });
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
        typedef exp::GenericNode<int*, nullptr, &location> test2;
        constexpr VFS<>& ptr2 = _vfs1;
        _vfs1.exists("test");
        ptr2.exists("test2");
        //typedef exp::GenericNode<VFS<>&, ptr2, ptr2> test3;
        typedef exp::GenericNode<blah&, blah1, blah1> test4;
        typedef exp::GenericNode<blah2<int>&, blah3, blah3> test5;
        //typedef exp::GenericNode<blah_base<int>&, blah3, blah3> test5; // this one doesn't work
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
