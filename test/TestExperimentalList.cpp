#include "catch.hpp"

#include "experimental/list.h"

namespace fstd = FactUtilEmbedded::std;

class TestNode : public FactUtilEmbedded::SinglyLinkedNode
{
public:
    int value;
};

SCENARIO( "Experimental std::list code", "[exp-list]" )
{
    fstd::experimental::forward_list<TestNode> list;

    WHEN("adding nodes")
    {
        TestNode node1, node2, node3;
        int counter = 0;

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
    }
}
