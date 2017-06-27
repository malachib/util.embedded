#include "catch.hpp"

#include "experimental/list.h"

//#include <list>

namespace util = FactUtilEmbedded;
namespace fstd = util::std;

template <class TNode>
class dynamic_node_allocator_base
{
    struct PtrNode : public TNode
    {
        void* value;
    };

public:
    typedef TNode node_type;
    typedef node_type* node_pointer;

    node_type* allocate(void *reference)
    {
        auto node = new PtrNode;

        node->value = reference;

        return node;
    }

    void deallocate(node_type* node)
    {
        // Maybe setting node->next to null here would be prudent?
        delete node;
    }

    static void* get_associated_value(node_type* node, const void* hint)
    {
        if(node == nullptr) return nullptr;

        return static_cast<PtrNode*>(node)->value;
    }

};

template <class T>
struct dynamic_node_allocator : public dynamic_node_allocator_base<util::SinglyLinkedNode>
{
public:
    typedef T value_type;
    typedef value_type* value_pointer;

    inline node_type* allocate(T *reference)
    {
        return dynamic_node_allocator_base::allocate(reference);
    }

    static inline T* get_associated_value(node_type* node, const void* hint)
    {
        return static_cast<T*>(dynamic_node_allocator_base::get_associated_value(node, hint));
    }
};

class TestNode2;

namespace FactUtilEmbedded { namespace std { namespace experimental {
template <>
struct node_allocator<TestNode2, SinglyLinkedNode> : public dynamic_node_allocator<TestNode2> {};
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

    class TestNode3 : public FactUtilEmbedded::DoublyLinkedNode
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
    WHEN("Trying doubly linked")
    {
        TestNode3 node1, node2, node3;

        node1.value = 10;
        node2.value = 20;
        node3.value = 30;

        fstd::experimental::list<TestNode3> list4;

        list4.push_front(node3);
        list4.push_front(node2);
        list4.push_front(node1);

        auto i = list4.begin();

        REQUIRE((*i++).value == 10);
        REQUIRE((*i++).value == 20);
        REQUIRE((*i).value == 30);


        for(auto n : list4)
        {

        }

        i = list4.begin();

        list4.erase_after(i);

        REQUIRE((*i++).value == 10);
        REQUIRE((*i).value == 30);
    }
    GIVEN("Replacement functions")
    {
        TestNode node1, node2, node3;
        TestNode node4;

        node1.value = 10;
        node2.value = 20;
        node3.value = 30;
        node4.value = 40;

        list.push_front(node3);
        list.push_front(node1);
        list.insert_after(list.begin(), node2);

        auto i = list.begin();

        i++;

        list.replace_after(i, node4);

        i = list.begin();

        REQUIRE((*i++).value == 10);
        REQUIRE((*i++).value == 20);
        REQUIRE((*i).value == 40);

        i = list.begin();

        list.erase_after(i);

        REQUIRE((*i++).value == 10);
        REQUIRE((*i).value == 40);
    }
}
