#include "catch.hpp"

#include "experimental/list.h"

#include <list>

namespace fstd = FactUtilEmbedded::std;

namespace util = FactUtilEmbedded;

class dynamic_node_allocator_base
{
    struct PtrNode : public util::SinglyLinkedNode
    {
        void* value;
    };

public:
    util::SinglyLinkedNode* allocate(void *reference)
    {
        auto node = new PtrNode;

        node->value = reference;

        return node;
    }

    void deallocate(util::SinglyLinkedNode* node)
    {
        // Maybe setting node->next to null here would be prudent?
        delete node;
    }

    void* get_associated_value(util::SinglyLinkedNode* node, const void* hint)
    {
        if(node == nullptr) return nullptr;

        return static_cast<PtrNode*>(node)->value;
    }

};

template <class T>
struct dynamic_node_allocator : public dynamic_node_allocator_base
{
public:
    inline util::SinglyLinkedNode* allocate(T *reference)
    {
        return dynamic_node_allocator_base::allocate(reference);
    }

    inline T* get_associated_value(util::SinglyLinkedNode* node, const void* hint)
    {
        return static_cast<T*>(dynamic_node_allocator_base::get_associated_value(node, hint));
    }
};

class TestNode2;

namespace FactUtilEmbedded { namespace std { namespace experimental {
template <>
struct node_allocator<TestNode2> : public dynamic_node_allocator<TestNode2> {};
}}}


SCENARIO( "Experimental std::list code", "[exp-list]" )
{
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


    fstd::experimental::forward_list<TestNode> list;
    //fstd::experimental::forward_list<TestNode2> list2; // this *does* work if TestNode2 is outside the function scope
    fstd::experimental::forward_list<TestNode2, dynamic_node_allocator<TestNode2>> list2;

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

        TestNode& copy = *list.begin();

        REQUIRE(copy.value == node1.value);
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
    WHEN("Trying primitive types")
    {
        //std::list<int> list3 = { 1, 2, 3, 4, 5 };
        fstd::experimental::forward_list<int, dynamic_node_allocator<int>> list3;

        int node1 = 10;
        int node2 = 20;
        int node3 = 30;

        list3.push_front(node3);
        list3.push_front(node1);
        list3.insert_after(list3.begin(), node2);

        for(auto i: list3)
        {
            REQUIRE(i == (counter += 10));
        }

        REQUIRE(list3.front() == 10);

        auto i2 = list3.begin();

        i2++;
    }
}
