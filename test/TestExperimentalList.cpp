#include "catch.hpp"

#include "experimental/list.h"

namespace fstd = FactUtilEmbedded::std;

class TestNode : public FactUtilEmbedded::SinglyLinkedNode
{
public:
    int value;
};

class TestNode2
{
public:
    int value;
};


template <>
struct fstd::experimental::node_allocator<TestNode2>
{
    struct PtrNode : public SinglyLinkedNode
    {
        TestNode2* value;
    };

public:
    SinglyLinkedNode* allocate(TestNode2 *reference)
    {
        auto node = new PtrNode;

        node->value = reference;

        return node;
    }

    void deallocate(SinglyLinkedNode* node)
    {
        // Maybe setting node->next to null here would be prudent?
        delete node;
    }

    TestNode2* get_associated_value(SinglyLinkedNode* node, const void* hint)
    {
        if(node == nullptr) return nullptr;

        return static_cast<PtrNode*>(node)->value;
    }
};



SCENARIO( "Experimental std::list code", "[exp-list]" )
{
    fstd::experimental::forward_list<TestNode> list;
    fstd::experimental::forward_list<TestNode2, fstd::experimental::node_allocator<TestNode2>> list2;

    int counter = 0;

    WHEN("adding nodes")
    {
        TestNode node1, node2, node3;

        node1.value = 10;
        node2.value = 20;
        node3.value = 30;

        list.push_front(node3);
        list.push_front(node1);
        list.insert_after(list.begin(), node2);

        for(auto i: list)
        {
            REQUIRE(i.value == (counter += 10));
        }

        REQUIRE(list.front().value == 10);
    }
    WHEN("adding nodes - specialized allocator")
    {
        TestNode2 node1, node2, node3;

        node1.value = 10;
        node2.value = 20;
        node3.value = 30;

        list2.push_front(node3);
        list2.push_front(node1);
        list2.insert_after(list2.begin(), node2);

        for(auto i: list2)
        {
            REQUIRE(i.value == (counter += 10));
        }

        REQUIRE(list2.front().value == 10);
    }
}
