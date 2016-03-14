#include "catch.hpp"

#include "LinkedList.h"
#include "fact/lib.h"
#include "fact/Menu.h"

using namespace util;

struct TestNode : public SinglyLinkedNode
{
  std::string s;
};


struct DummyMenu2 : public MenuGeneric
{
  static int counter;
  //char* buffer, *buffer2;

  DummyMenu2()
  {
    char* buffer = new char[128];

    char* buffer2 = new char[128];
    sprintf(buffer, "Menu%d", counter);
    sprintf(buffer2, "Menu desc %d", counter++);
    setName(buffer);
    setDescription(buffer2);
  }

  ~DummyMenu2()
  {
    // FIX: free up that memory
  }
};

int DummyMenu2::counter = 1;

SCENARIO("LinkedList handler", "[linkedlist]")
{
  GIVEN("A simplest-case linked list")
  {
    INFO("Prepping");
    SinglyLinkedList ll;
    TestNode n1;
    n1.s = "test1";
    TestNode n2;
    n2.s = "test2";

    INFO("Adding");
    ll.add(&n1);
    ll.add(&n2);

    TestNode* head = (TestNode*) ll.getHead();
    REQUIRE(head->s == "test1");
    auto next = (TestNode*) head->getNext();
    REQUIRE(next->s == "test2");
  }
  GIVEN("A more complicated menu linked list")
  {
    INFO("Prepping");
    SinglyLinkedList ll;
    /*
    MenuGeneric n1;
    n1.setDescription("Menu 1 desc");
    n1.setName("Menu1");
    MenuGeneric n2;
    n2.setDescription("Menu 2 desc");
    n2.setName("Menu2");*/
    DummyMenu2 n1, n2;
    INFO("Adding");
    std::string stackRefS = (char*)n1.getName();
    REQUIRE(n1.getName() != NULL);
    REQUIRE(stackRefS == "Menu1");
    ll.add(&n1);
    stackRefS = (char*)n1.getName();
    REQUIRE(stackRefS == "Menu1");
    ll.add(&n2);
    MenuCommand* head = (MenuCommand*) ll.getHead();
    REQUIRE(head == &n1);
    REQUIRE(head->getNext() == &n2);
    INFO("Retrieving name");
    REQUIRE(head->getName() != NULL);
    stackRefS = (char*)n1.getName();
    std::string s = (char*)head->getName();
    REQUIRE(stackRefS == s);
    REQUIRE(s == "Menu1");
    REQUIRE(stackRefS == "Menu1");
    auto next = (MenuCommand*) head->getNext();
    s = (char*)next->getName();
    REQUIRE(s == "Menu2");

  }
}
